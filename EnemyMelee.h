// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyMelee : public AEnemyBase
{
	GENERATED_BODY()
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
private:

public:

	AEnemyMelee();

	UFUNCTION(BlueprintCallable)
	void AttackBegin();

	UFUNCTION(BlueprintImplementableEvent)
	void Attack();
	UFUNCTION(BlueprintImplementableEvent)
	void Move(AActor* Target,float AcceptableRadius);
	UFUNCTION(BlueprintCallable)
	void SaveAttack();
	UFUNCTION(BlueprintCallable)
	void ResetCombo();

	UPROPERTY(BlueprintReadWrite)
	bool InRange = false;
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite)
	int AttackCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent* HitPoint = nullptr;

};
