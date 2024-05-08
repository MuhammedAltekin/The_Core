// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTank.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)


AEnemyTank::AEnemyTank()
{
	TeleportLocation = CreateDefaultSubobject<UArrowComponent>(FName("Teleport Location"));
	TeleportLocation->SetupAttachment(RootComponent);
	HitPoint = CreateDefaultSubobject<UArrowComponent>(FName("Hit Point"));
	HitPoint->SetupAttachment(RootComponent);
}

void AEnemyTank::BeginPlay()
{
	Super::BeginPlay();
	

}

void AEnemyTank::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ProtectionPrint = ProtectionTargetType;
	if (IGameState)
	{

		if (!bSupportMode)
		{
			//Rage Trigger
			if (!OnceRage && DistanceToTarget < 1250)
			{
				OnceRage = true;
				OnceTimer(RageTriggerTH, RageTrigger();, false, 1, .2f);
			}
			IsShield = false;
			ShieldCollisionSetting(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->MaxWalkSpeed = 600;
			if (IsRaged && !InRange)
			{
				const float SelectedRadius = UKismetMathLibrary::SelectFloat(Range * 1.5f, Range, PlayerGameState->Triggered);
				Move(SelectedRadius);
				TurnToTarget = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
			if (!IsRaged)
			{
				AIController->MoveToActor(PlayerGameState->GetTarget());
			}
		}
		else
		{

			IsShield = true;
			ShieldCollisionSetting(ECollisionEnabled::QueryOnly);
			GetCharacterMovement()->MaxWalkSpeed = 400;
			TurnToTarget = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			if (AIController)
			{
				AIController->MoveToLocation(FindGuardLocation(), 100, false);
			}

		}
	}
}

FVector AEnemyTank::FindGuardLocation()
{
	FVector FinalLocation;
	//Check Enemy Priority and Amount
	for (int i = 0; i <= 5; i++)
	{
		switch (i)
		{
			//Target Priority
		case 0:
			ProtectionTargetType = EEnemies::Boss;
			
			break;
		case 1:
			ProtectionTargetType = EEnemies::Bomber;
			
			break;
		case 2:
			ProtectionTargetType = EEnemies::Ranged;
			
			break;
		case 3:
			ProtectionTargetType = EEnemies::Long_Ranged;
			
			break;
		case 4:
			ProtectionTargetType = EEnemies::Melee;
			
			break;
		case 5:
			ProtectionTargetType = EEnemies::Buffer;
			
			break;
		default:;
			break;
		}

		if (IGameState->GetSetEnemyAmount(ProtectionTargetType, true, false, false, EEnemies::Bomber) > 0)
		{
			break;
		}
	}
	//Select Protection Target Type
	switch (ProtectionTargetType)
	{
	case 0: //Bomber
		ProtectionTargetActorType = BomberActor;
		break;
	case 1:	//Long Ranged	
		ProtectionTargetActorType = LongRangedActor;
		break;
	case 2:	//Boss
		ProtectionTargetActorType = BossActor;
		break;
	case 3:	//Melee
		ProtectionTargetActorType = MeleeActor;
		break;
	case 4:	//Ranged
		ProtectionTargetActorType = RangedActor;
		break;
	case 6: //Buffer
		ProtectionTargetActorType = BufferActor;
		break;

	default:;
	}

	//Find Nearest Actor & Calculate Protection Position
	float NearestActorDistance;
	const AActor* NearestActor = nullptr;
	FRotator LookAtRot;

	if (PlayerGameState->Player)
	{
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ProtectionTargetActorType, OutActors);

		NearestActor = UGameplayStatics::FindNearestActor(PlayerGameState->Player->GetActorLocation(), OutActors, NearestActorDistance);
		if (NearestActor)
		{
			LookAtRot = UKismetMathLibrary::FindLookAtRotation(NearestActor->GetActorLocation(), PlayerGameState->Player->GetActorLocation());
		}


	}
	if (NearestActor)
	{
		FinalLocation = NearestActor->GetActorLocation() + ((NearestActorDistance / 3) * UKismetMathLibrary::GetForwardVector(LookAtRot));
	}
	
	return FinalLocation;
}

void AEnemyTank::RageEnd()
{
	SetActorLocation(TeleportLocation->GetComponentLocation(), false);
	IsRaged = true;
	InRange = false;
}

void AEnemyTank::AttackTrigger()
{
	if (!IsAttacking)
	{
		IsAttacking = true;
		Attack();
	}
}

void AEnemyTank::AttackSave()
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

void AEnemyTank::ResetAttack()
{
	AttackCount = 0;
	IsAttacking = false;
	InRange = false;
}
