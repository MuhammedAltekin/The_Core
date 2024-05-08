// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBuffer.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyBuffer : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyBuffer();

	

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	//FUNCTIONS
	UFUNCTION(BlueprintImplementableEvent)
	void BufferProtectionMove(FVector Destination);
	UFUNCTION(BlueprintImplementableEvent)
	void Move();
	UFUNCTION(BlueprintCallable)
	void AllyCheckAndMove();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void SaveCombo();
	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	const AActor* GetWhoNeedProtection();


	//Class Defaults
	UPROPERTY(VisibleAnywhere)
	bool ProtectionOn;
	UPROPERTY(VisibleAnywhere)
	bool ProtectionOff;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Shield;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent* ShieldParticle;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ProtectionOnVFX;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ProtectionOffVFX;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackAnim;
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AActor> ProtectionTargetActorType;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BossActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> MeleeActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RangedActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> LongRangedActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BomberActor;
	UPROPERTY(BlueprintReadWrite)
	bool bAllyCheckOnce;
	UPROPERTY(VisibleAnywhere)
	bool ProtectionMod = false;
	UPROPERTY(BlueprintReadWrite)
	bool InRange;

	
	bool IsAttacking;
	EEnemies ProtectionTargetType;
	
};
