// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLongRanged.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

void AEnemyLongRanged::BeginPlay()
{
	Super::BeginPlay();

	
}

void AEnemyLongRanged::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!InRange)
	{
		MoveToFunc();
		TurnToTarget = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void AEnemyLongRanged::SaveCombo()
{
	if (!IsDead && PlayerGameState->GetTarget() && DistanceToTarget < Range)
	{
		AttackTrigger();
	}
}

void AEnemyLongRanged::AttackTrigger()
{
	if (DistanceToTarget < Range / 2)
	{
		Primary();
	}
	else
	{
		Cannon();
	}
}

void AEnemyLongRanged::Attack()
{
	if (!IsAttacking && !IsDead)
	{
		IsAttacking = true;
		AttackTrigger();
	}
}

void AEnemyLongRanged::ResetAttack()
{
	IsAttacking = false;
	InRange = false;
}

