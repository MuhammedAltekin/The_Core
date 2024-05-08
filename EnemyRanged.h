// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyRanged.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyRanged : public AEnemyBase
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MoveToFunc();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();
	UFUNCTION(BlueprintCallable)
	void SaveAttack();
	UFUNCTION(BlueprintCallable)
	void AttackTrigger();

	UPROPERTY(BlueprintReadWrite)
	bool InRange;

	bool IsAttacking;


};
