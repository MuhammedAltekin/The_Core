// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DamageNumbersActor.h"
#include "GameFramework/Character.h"
#include "Enemies.h"
#include "NanocellCapsule.h"
#include "NiagaraSystem.h"
#include "PlayerGameState.h"

#include "EnemyBase.generated.h"


UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//FUNCTIONS

	UFUNCTION(BlueprintCallable)
	void DeathEvent();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void TurnTarget();
	void RotatingToTarget();
	
	//Variable
	UPROPERTY(BlueprintReadOnly)
	APlayerGameState* PlayerGameState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	UPROPERTY(EditAnywhere)
	float Armor;
	UPROPERTY(EditDefaultsOnly)
	float DroppedExperience;
	UPROPERTY(EditAnywhere)
	float CapsuleDropPossibility;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float DistanceToTarget;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool TurnToTarget;
	UPROPERTY(BlueprintReadOnly)
	bool IsDead;
	UPROPERTY(EditAnywhere)
	bool IsRotating;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	bool bSupportMode;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bBoss;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AAIController* AIController;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DeathSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* DeathDialogSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* WingSound;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* ShieldSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* WingAudio;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* ShieldAudio;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ANanocellCapsule> NanocellCapsuleActor;
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem* DeathFX;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EEnemies> EnemyType;
	UPROPERTY(BlueprintReadWrite)
	FVector IncomingDamageLocation;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADamageNumbersActor> DamageNumbersActor;

	FTimerHandle RotatingTh;
	IGameStateInterface* IGameState;
	FTimerHandle DeadTh;

};
