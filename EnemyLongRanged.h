// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyLongRanged.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyLongRanged : public AEnemyBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Primary();
	UFUNCTION(BlueprintImplementableEvent)
	void Cannon();
	UFUNCTION(BlueprintImplementableEvent)
	void MoveToFunc();

	void AttackTrigger();

	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void SaveCombo();
	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	
	UPROPERTY(BlueprintReadWrite)
	bool InRange;

	bool IsAttacking;
};
