// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class INTRUDERPROTO_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	FORCEINLINE float GetMaxHealthPoints() const { return MaxHealthPoints; }

	FORCEINLINE float GetCurentHealthPoints() const { return CurentHealthPoints; }

	FORCEINLINE bool IsAlive() const { return CurentHealthPoints > 0; }

	void StartRunning();

	void StopRunning();

protected:
	/* Space inside which we can hit our target */
	UPROPERTY(VisibleAnywhere, Category = "Attacking")
	UCapsuleComponent* MeleeCollisionComp;

	/* Timer handle to manage continous melee attacks while in range of a player */
	UPROPERTY()
	FTimerHandle TimerHandle_MeleeAttack;

	/* Minimum time between melee attacks */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking", meta = (AllowPrivateAccess = "true"))
	float MeleeStrikeCooldown;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	/* Deal damage to the Actor that was hit by the punch animation */
	UFUNCTION(BlueprintCallable, Category = "Attacking")
	void PerformMeleeStrike(AActor* HitActor);

	virtual bool Die(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	virtual void OnDeath(float KillingDamage, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser);

	void SetRagdollPhysics();

	/* A pawn is in melee range */
	UFUNCTION()
	virtual void OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnRetriggerMeleeStrike();

	/** Called via input to crouch or uncrouch. */
	void ToggleCrouch();

private:
	/* Maximum amount of HP a character can have before dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealthPoints;

	/* Amount of HP a character has at the begining of the game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float StartingHealthPoints;

	/* Curent amount of HP a character has */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float CurentHealthPoints;

	/* Is the character dying? */
	UPROPERTY()
	bool bIsDying;

	/* Last time we attacked something */
	UPROPERTY()
	float LastMeleeAttackTime;

	/* Type of damage */
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	TSubclassOf<UDamageType> PunchDamageType;

	/* Amount of damage */
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
	float MeleeDamage;

	/* Timer handle to manage buffered melee attacks */
	UPROPERTY()
	FTimerHandle TimerHandle_MeleeAttackOnHold;

	/* Tracks noise data used by the pawn sensing component */
	UPROPERTY()
	UPawnNoiseEmitterComponent* NoiseEmitterComp;


	//////////////////////////
	// Movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float WalkingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float RunningSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsRunning;
};