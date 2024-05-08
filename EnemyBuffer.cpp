// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBuffer.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)


AEnemyBuffer::AEnemyBuffer()
{
	Shield = CreateDefaultSubobject<UStaticMeshComponent>(FName("Shield"));
	Shield->SetupAttachment(RootComponent);

	ShieldParticle = CreateDefaultSubobject<UParticleSystemComponent>(FName("Shield Particle"));
	ShieldParticle->SetupAttachment(RootComponent);

}

void AEnemyBuffer::BeginPlay()
{
	Super::BeginPlay();

	
}

void AEnemyBuffer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsDead)
	{
		ShieldParticle->SetActive(false, false);
		Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{

		if (bSupportMode)
		{
			ProtectionMod = true;
			AllyCheckAndMove();

			TurnToTarget = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
		}
		else
		{
			ProtectionMod = false;

			if (!InRange)
			{
				Move();
				TurnToTarget = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
		}
	}

	//Switch Mode
	if (ProtectionMod)
	{
		ProtectionOff = false;
		if (!ProtectionOn)
		{
			ProtectionOn = true;

			UGameplayStatics::SpawnEmitterAttached(ProtectionOnVFX, GetMesh(), FName("None"), FVector(0, 0, -75), FRotator(0), FVector(1.5f, 1.5f, .5f), EAttachLocation::KeepRelativeOffset);
			FTimerHandle OnceTH;
			OnceTimer(OnceTH,
				ShieldParticle->SetActive(true);
			Shield->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			, false, 1, 1);
		}
	}
	else
	{
		ProtectionOn = false;
		if (!ProtectionOff)
		{
			ProtectionOff = true;
			FTimerHandle OnceTH;
			OnceTimer(OnceTH,
				UGameplayStatics::SpawnEmitterAttached(ProtectionOffVFX, GetMesh(), FName("None"), FVector(0, 0, -75), FRotator(0), FVector(1.5f, 1.5f, .5f), EAttachLocation::KeepRelativeOffset, true);
			ShieldParticle->SetActive(false);
			Shield->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			, false, 1, 1);
		}
	}


}

const AActor* AEnemyBuffer::GetWhoNeedProtection()
{
	//Check Enemy Priority and Amount
	for (int i = 0; i <= 4; i++)
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

	default:;
	}

	//Find Nearest Actor & Calculate Protection Position
	const AActor* NearestActor = nullptr;

	if (PlayerGameState->Player)
	{
		float NearestActorDistance;
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ProtectionTargetActorType, OutActors);
		NearestActor = UGameplayStatics::FindNearestActor(PlayerGameState->Player->GetActorLocation(), OutActors, NearestActorDistance);
	}

	return NearestActor;
}

void AEnemyBuffer::Attack()
{
	if (!IsAttacking && !IsDead)
	{
		IsAttacking = true;
		PlayAnimMontage(AttackAnim, 1);
	}
}

void AEnemyBuffer::SaveCombo()
{
	if (!IsDead && PlayerGameState->GetTarget() && DistanceToTarget < Range)
	{
		PlayAnimMontage(AttackAnim, 1);
	}
}

void AEnemyBuffer::ResetCombo()
{
	IsAttacking = false;
	InRange = false;
}

void AEnemyBuffer::AllyCheckAndMove()
{
	if (!bAllyCheckOnce)
	{
		bAllyCheckOnce = true;
		if (GetWhoNeedProtection())
		{
			FVector TargetLocation;
			UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(GetWorld(), GetWhoNeedProtection()->GetActorLocation(), TargetLocation, 500);
			BufferProtectionMove(TargetLocation);
		}
		else
		{
			bAllyCheckOnce = false;
		}
	}

}
