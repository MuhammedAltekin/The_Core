// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyRanged.h"

#include "GameFramework/CharacterMovementComponent.h"

#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

void AEnemyRanged::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyRanged::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!InRange)
	{
		MoveToFunc();
		TurnToTarget = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void AEnemyRanged::AttackTrigger()
{
	if (!IsAttacking && !IsDead)
	{
		IsAttacking = true;
		Attack();
	}
}

void AEnemyRanged::ResetAttack()
{
	InRange = false;
	IsAttacking = false;
}

void AEnemyRanged::SaveAttack()
{
	if (!IsDead && PlayerGameState->GetTarget())
	{
		if (DistanceToTarget < Range)
		{
			Attack();
		}
	}
}

