// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyMelee.h"
#include "AIController.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

AEnemyMelee::AEnemyMelee()
{
	HitPoint = CreateDefaultSubobject<UArrowComponent>(FName("Hit Point"));
	HitPoint->SetupAttachment(RootComponent);
}

void AEnemyMelee::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyMelee::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!InRange && PlayerGameState->GetTarget())
	{
		const float SelectedRange = UKismetMathLibrary::SelectFloat(Range * 1.5f, Range, PlayerGameState->Triggered);
		Move(PlayerGameState->GetTarget(), SelectedRange - 100);
		TurnToTarget = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
	}
}

void AEnemyMelee::AttackBegin()
{
	if (!IsAttacking && !IsDead)
	{
		IsAttacking = true;
		Attack();
	}
	
}


void AEnemyMelee::SaveAttack()
{
	if (!IsDead && PlayerGameState->GetTarget())
	{
		const float SelectedRange = UKismetMathLibrary::SelectFloat(Range * 1.5f, Range, PlayerGameState->Triggered);
		const bool DistanceCheck = UKismetMathLibrary::Less_DoubleDouble(DistanceToTarget, SelectedRange);
		if (DistanceCheck)
		{
			Attack();
		}
	}
}

void AEnemyMelee::ResetCombo()
{
	AttackCount = 0;
	IsAttacking = false;
	InRange = false;
}


