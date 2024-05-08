// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBomber.h"

#include "GameFramework/CharacterMovementComponent.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)


AEnemyBomber::AEnemyBomber()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBomber::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBomber::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!InRange)
	{
		Move();
		TurnToTarget = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void AEnemyBomber::Attack()
{
	if (!IsDead && !IsAttacking)
	{
		IsAttacking = true;
		AttackAnimation();
	}
}

void AEnemyBomber::SaveAttack()
{
	if (!IsDead && PlayerGameState->GetTarget() && DistanceToTarget < Range)
	{
		AttackAnimation();
	}
}

void AEnemyBomber::ResetCombo()
{
	IsAttacking = false;
	InRange = false;
}
