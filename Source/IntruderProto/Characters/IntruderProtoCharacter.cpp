// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "IntruderProtoCharacter.h"
#include "IntruderProtoProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Components/MyCharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Usables/Ladder.h"
#include "Usables/Throwable.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AIntruderProtoCharacter

//AIntruderProtoCharacter::AIntruderProtoCharacter()
AIntruderProtoCharacter::AIntruderProtoCharacter(const FObjectInitializer& ObjectInitializer) 
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
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	//setup the overlap delegate
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this,&AIntruderProtoCharacter::OnOverlapEnd);

	//init the grab handle
	GrabHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("GrabHandle"));

	//Init our values
	UsingReach = 200.0f;
	GrabDistance = 200.0f;
	ThrowForce = 20000.0f;
}

void AIntruderProtoCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AIntruderProtoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AIntruderProtoCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AIntruderProtoCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AIntruderProtoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AIntruderProtoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AIntruderProtoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AIntruderProtoCharacter::LookUpAtRate);

	// bind special actions
	PlayerInputComponent->BindAction("Crouch", IE_Pressed,this, &AIntruderProtoCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &AIntruderProtoCharacter::Use);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &AIntruderProtoCharacter::Throw);
}

void AIntruderProtoCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AIntruderProtoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AIntruderProtoProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AIntruderProtoCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AIntruderProtoCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AIntruderProtoCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AIntruderProtoCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AIntruderProtoCharacter::MoveForward(float Value)
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

void AIntruderProtoCharacter::MoveRight(float Value)
{
	if (Value != 0.0f) {
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AIntruderProtoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AIntruderProtoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AIntruderProtoCharacter::ToggleCrouch()
{
	if (bIsCrouched) {
		UnCrouch();
	}
	else {
		Crouch();
	}
}

void AIntruderProtoCharacter::Use()
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

void AIntruderProtoCharacter::Throw()
{
	AThrowable *Throwable = Cast<AThrowable>(OnUseUsable);
	if (Throwable) {
		Throwable->OnThrow(GetController());
		OnUseUsable = NULL;
	}
}

void AIntruderProtoCharacter::Jump()
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

bool AIntruderProtoCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AIntruderProtoCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AIntruderProtoCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AIntruderProtoCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}

void AIntruderProtoCharacter::Tick(float DeltaSeconds)
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

void AIntruderProtoCharacter::SetIsClimbingLadder(const bool &NewIsClimbingLadder)
{
	bIsClimbingLadder = NewIsClimbingLadder;

	if (!bIsClimbingLadder) {
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_NavWalking);
	}
}

void AIntruderProtoCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
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