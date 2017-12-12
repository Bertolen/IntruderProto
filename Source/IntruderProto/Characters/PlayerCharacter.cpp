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
#include "Components/MyCharacterMovementComponent.h"
#include "Components/InventoryComponent.h"
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

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	//FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->RelativeLocation = FVector(0.0f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);
	GetMesh()->RelativeRotation = FRotator(0.0f, 0.0f, -90.0f);

	//setup the overlap delegate
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);

	//init the grab handle
	GrabHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("GrabHandle"));

	// Init the inventory
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	// The player can crouch
	GetMovementComponent()->NavAgentProps.bCanCrouch = true;

	// Default offset from the character location for projectiles to spawn
	ItemSpawnOffset = FVector(100.0f, 0.0f, 10.0f);

	//Init our values
	UsingReach = 200.0f;
	GrabDistance = 200.0f;
	ThrowForce = 20000.0f;
	EquipedItem = nullptr;
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
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::StartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &APlayerCharacter::StopRunning);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::AddControllerPitchInput);

	// bind special actions
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &APlayerCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &APlayerCharacter::Use);
	PlayerInputComponent->BindAction("Throw", IE_Pressed, this, &APlayerCharacter::Throw);

	PlayerInputComponent->BindAxis("InventoryScroll", this, &APlayerCharacter::ScrollInventory);
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

void APlayerCharacter::ScrollInventory(float Value)
{
	if (Value > 0.0f) {

		if (EquipedItem == nullptr) { // In case there's no currently equiped item we get the first item in the inventory
			if (Inventory->GetUsedSlots() > 0)
			{
				EquipedItem = Inventory->GetItemClassAtIndex(0);
			}
		} 
		else { // If there's an equiped item then we get the next item in the inventory
			int index = Inventory->GetIndex(EquipedItem);
			if (index != INDEX_NONE) {
				if (Inventory->GetUsedSlots() > index + 1) {
					EquipedItem = Inventory->GetItemClassAtIndex(index + 1);
				}
				else {
					EquipedItem = nullptr;
				}
			}
		}

	}
	else if (Value < 0.0f) { 

		if (EquipedItem == nullptr) { // In case there's no currently equiped item we get the last item in the inventory
			if (Inventory->GetUsedSlots() > 0)
			{
				EquipedItem = Inventory->GetItemClassAtIndex(Inventory->GetUsedSlots() - 1);
			}
		}
		else { // If there's an equiped item then we get the previous item in the inventory
			int index = Inventory->GetIndex(EquipedItem);
			if (index != INDEX_NONE) {
				if (index != 0) {
					EquipedItem = Inventory->GetItemClassAtIndex(index - 1);
				}
				else {
					EquipedItem = nullptr;
				}
			}
		}

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
	UE_LOG(LogClass, Log, TEXT("Throw"));
	AThrowable *Throwable = Cast<AThrowable>(OnUseUsable);
	if (Throwable) { // priority is given to the throwable item on use
		Throwable->OnThrow(GetController());
		OnUseUsable = NULL;
	}
	else if (EquipedItem != nullptr) { // If there is no throwable to throw then we use the equiped item
		UE_LOG(LogClass, Log, TEXT("using an equiped item"));
		UWorld* const World = GetWorld();
		if (World == NULL)
			return;

		const FRotator SpawnRotation = GetControlRotation();
		const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(ItemSpawnOffset);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// spawn the projectile at the muzzle
		APickup* SpawnedPickup = World->SpawnActor<APickup>(EquipedItem, SpawnLocation, SpawnRotation, ActorSpawnParams);
		if (!SpawnedPickup)
			return;

		UE_LOG(LogClass, Log, TEXT("Item has been spawned : %s"), *SpawnedPickup->GetFName().ToString());
		SpawnedPickup->OnReleased(GetController());

		// Record on the Inventory that we just released one of these
		if (!Inventory->RemoveItem(EquipedItem)) {
			EquipedItem = nullptr;
		}

	}
	UE_LOG(LogClass, Log, TEXT("Throw end"));
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

void APlayerCharacter::MakePawnNoise(float Loudness)
{
	if (Role == ROLE_Authority)
	{
		/* Make noise to be picked up by PawnSensingComponent by the enemy pawns */
		MakeNoise(Loudness, this, GetActorLocation());
	}

	//LastNoiseLoudness = Loudness;
	//LastMakeNoiseTime = GetWorld()->GetTimeSeconds();
}