// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBomber.generated.h"


/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyBomber : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyBomber();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void Move();
	UFUNCTION(BlueprintImplementableEvent)
	void AttackAnimation();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	void SaveAttack();
	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	
	UPROPERTY(BlueprintReadWrite)
	bool InRange = false;

	bool IsAttacking = false;
};
