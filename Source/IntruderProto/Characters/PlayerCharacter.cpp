// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "IntruderProtoProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "MyCharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Usables/Ladder.h"
#include "Usables/Throwable.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

//APlayerCharacter::APlayerCharacter()
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	//FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->RelativeLocation = FVector(0.0f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);
	GetMesh()->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	/*
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	*/

	//setup the overlap delegate
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

	//init the grab handle
	GrabHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("GrabHandle"));

	//Init our values
	UsingReach = 200.0f;
	GrabDistance = 200.0f;
	ThrowForce = 20000.0f;
}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

}

//////////////////////////////////////////////////////////////////////////
// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlayerCharacter::LookUpAtRate);

	// bind special actions
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &APlayerCharacter::Use);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &APlayerCharacter::Throw);
}

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		if (bIsClimbingLadder) {
			// if climbing add movement upwards
			AddMovementInput(GetActorUpVector(), Value);
		}
		else {
			// if not, add movement forwards
			AddMovementInput(GetActorForwardVector(), Value);
		}
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f) {
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::ToggleCrouch()
{
	if (bIsCrouched) {
		UnCrouch();
	}
	else {
		Crouch();
	}
}

void APlayerCharacter::Use()
{
	AUsable* OldUsable = NULL;
	// Starts by releasing the currently used object
	if (OnUseUsable != NULL) {
		OnUseUsable->OnReleased(GetController());
		OldUsable = OnUseUsable;
		OnUseUsable = NULL;

		if (bIsClimbingLadder) {
			SetIsClimbingLadder(false);
		}
	}

	// we access the usable item, make sure we have one, else we will crash
	if (FocusedUsable != NULL && FocusedUsable != OldUsable && FocusedUsable->CanBeUsed(GetController())) {
		OnUseUsable = FocusedUsable;

		// If the call returns false then the object does not stays in use
		if (!OnUseUsable->OnUsed(Controller)) {
			OnUseUsable = NULL;
		}
	}
}

void APlayerCharacter::Throw()
{
	AThrowable *Throwable = Cast<AThrowable>(OnUseUsable);
	if (Throwable) {
		Throwable->OnThrow(GetController());
		OnUseUsable = NULL;
	}
}

void APlayerCharacter::Jump()
{
	if (bIsClimbingLadder) {
		SetIsClimbingLadder(false);
		OnUseUsable->OnReleased(GetController());
		OnUseUsable = NULL;
	}
	else {
		Super::Jump();
	}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CamLoc;
	FRotator CamRot;

	Controller->GetPlayerViewPoint(CamLoc, CamRot); // Get the camera position and rotation

													// we check the controller because we dont want bots to grab the use object and we need a controller for the Getplayerviewpoint function
	if (Controller && Controller->IsLocalPlayerController()) {
		const FVector StartTrace = CamLoc; // trace start is the camera location
		const FVector Direction = CamRot.Vector();
		const FVector EndTrace = StartTrace + Direction * UsingReach; // and trace end is the camera location + an offset in the direction you are looking, the 200 is the distance at wich it checks

																	  // Perform trace to retrieve hit info
		FCollisionQueryParams TraceParams(FName(TEXT("WeaponTrace")), true, this);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		FHitResult Hit(ForceInit);

		// simple trace function
		GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECollisionChannel::ECC_Visibility, TraceParams);

		// we cast the hit actor to the AUsable Actor
		AUsable* usable = Cast<AUsable>(Hit.GetActor());

		// we are looking to a usable object
		if (usable) {
			// as the actor under crosshairs is a usable actor, we store it for the hud.
			FocusedUsable = usable;
		}
		else {
			// nothing, so we set the UseFocus pointer to NULL, so it wont give problems
			FocusedUsable = NULL;
		}
	}

	//Update the grad handle position
	FVector NewGrabLocation = CamLoc + CamRot.Vector() * GrabDistance;
	GrabHandle->SetTargetLocationAndRotation(NewGrabLocation, CamRot);
}

void APlayerCharacter::SetIsClimbingLadder(const bool &NewIsClimbingLadder)
{
	bIsClimbingLadder = NewIsClimbingLadder;

	if (!bIsClimbingLadder) {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	}
}

void APlayerCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bIsClimbingLadder) {
		ALadder* Ladder = Cast<ALadder>(OtherActor);
		UBoxComponent* climbVolume = Cast<UBoxComponent>(OtherComp);
		if (climbVolume && Ladder && Ladder == Cast<ALadder>(OnUseUsable)) {
			SetIsClimbingLadder(false);
			OnUseUsable->OnReleased(GetController());
			OnUseUsable = NULL;
		}
	}
}