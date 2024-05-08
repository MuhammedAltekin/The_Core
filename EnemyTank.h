// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyTank.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyTank : public AEnemyBase
{
	GENERATED_BODY()

protected:

	AEnemyTank();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Attack();
	UFUNCTION(BlueprintImplementableEvent)
	void RageTrigger();
	UFUNCTION(BlueprintImplementableEvent)
	void ShieldCollisionSetting(ECollisionEnabled::Type CollisionType);
	UFUNCTION(BlueprintImplementableEvent)
	void Move(float AcceptableRange);
	UFUNCTION(BlueprintCallable)
	void RageEnd();
	UFUNCTION(BlueprintCallable)
	void AttackSave();
	UFUNCTION(BlueprintCallable)
	void ResetAttack();
	UFUNCTION(BlueprintCallable)
	void AttackTrigger();
	FVector FindGuardLocation();

private:

public:

	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* TeleportLocation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* HitPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BossActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BufferActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> MeleeActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> RangedActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> LongRangedActor;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BomberActor;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	bool InRange;
	UPROPERTY(VisibleAnywhere)
	bool IsRaged;
	UPROPERTY(BlueprintReadWrite)
	bool IsShield = true;
	UPROPERTY(BlueprintReadWrite)
	int AttackCount;
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AActor> ProtectionTargetActorType;
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EEnemies> ProtectionPrint;

	bool OnceRage;
	bool IsAttacking;
	FTimerHandle RageTriggerTH;
	EEnemies ProtectionTargetType;
};
