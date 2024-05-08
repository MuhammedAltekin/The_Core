// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBoss.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define String(Text) FString(TEXT(Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

void AEnemyBoss::BeginPlay()
{
	Super::BeginPlay();
	
	OnceTimer(OnceTHandle,
		CooldownCounting();
	,true,.1f,0);
	PlayerGameState->WholeTimers.Add(OnceTHandle);
}

void AEnemyBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (PlayerGameState->Triggered)
	{
		if (!bInRange && !bSkillUsing)
		{
			TurnToTarget = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = true;
			MoveToLoc();
		}
	}
	else
	{

		if (bSkillReady)
		{
			UseSkill();
			TurnToTarget = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
		}
		else
		{
			if (!bInRange && !bSkillUsing)
			{
				MoveToLoc();
				TurnToTarget = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
		}
	}

}

void AEnemyBoss::CooldownCounting()
{
	//Checking Skill Ready
	if (bHookReady || bShockBlastReady || bStomachGunReady || bEarthquakeReady)
	{
		bSkillReady = true;
	}
	else
	{
		bSkillReady = false;
	}


	//Hook Cooldown
	HookCooldown -= 0.1f;
	if (HookCooldown <= 0 && DistanceToTarget >= 750)
	{
		bHookReady = true;
	}
	else
	{
		bHookReady = false;
	}

	//Shock Blast Cooldown
	ShockBlastCooldown -= 0.1f;
	if (ShockBlastCooldown <= 0 && DistanceToTarget <= 500)
	{
		bShockBlastReady = true;
	}
	else
	{
		bShockBlastReady = false;
	}

	//Stomach Gun Cooldown
	StomachGunCooldown -= 0.1f;
	if (StomachGunCooldown <= 0)
	{
		bStomachGunReady = true;
	}
	else
	{
		bStomachGunReady = false;
	}

	//Earthquake Cooldown
	EarthquakeCooldown -= 0.1f;
	if (EarthquakeCooldown <= 0)
	{
		bEarthquakeReady = true;
	}
	else
	{
		bEarthquakeReady = false;
	}
}

void AEnemyBoss::Attack()
{
	GetCharacterMovement()->StopMovementImmediately();
	if (!bIsAttacking)
	{
		bIsAttacking = true;
		AttackTrigger();
	}
}


void AEnemyBoss::UseHook()
{
	if (bHookReady && !bSkillUsing)
	{
		if (DistanceToTarget <= 1500)
		{
			bSkillUsing = true;
			bHookReady = false;
			HookCooldown = DesiredHookCooldown;
			PlayAnimMontage(HookBeginAnim);
			GetCharacterMovement()->StopMovementImmediately();
		}
		else
		{
			AIController->MoveToActor(PlayerGameState->GetTarget(),1450,false);
		}
	}
}

void AEnemyBoss::HookBeginEvent()
{
	bIsHookBegan = true;
}

void AEnemyBoss::HookPullEvent()
{
	bIsHookBegan = false;
	bIsHookPull = true;
	PlayAnimMontage(HookPullAnim);
	if (!PlayerGameState->Player->SlowMotion)
	{
		PlayerGameState->Player->Knockback = true;
		PlayerGameState->DisableInput(UGameplayStatics::GetPlayerController(GetWorld(),0));
		const FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerGameState->Player->GetActorLocation());
		SetActorRotation(NewRot);

		
		const FVector PushForce = 10 * UKismetMathLibrary::GetForwardVector(NewRot) * (DistanceToTarget * UKismetMathLibrary::MapRangeClamped(DistanceToTarget,0,2750,1,0));
		PlayerGameState->Player->GetCharacterMovement()->AddImpulse(PushForce, true);
		UGameplayStatics::ApplyDamage(PlayerGameState->Player, PlayerGameState->GetBossHookDamage_Implementation(),GetController(),this,nullptr);

		const FVector PullForce = 5 *	UKismetMathLibrary::NegateVector(UKismetMathLibrary::GetForwardVector(NewRot)) * DistanceToTarget;
		PullImplementEvent(PullForce);
	}

}

void AEnemyBoss::HookPullBlast(float PushForce, float Damage)
{
	const FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerGameState->Player->GetActorLocation());
	const FVector CalculatedForce = PushForce * UKismetMathLibrary::GetForwardVector(NewRot);
	PlayerGameState->Player->GetCharacterMovement()->AddImpulse(CalculatedForce, true);
	UGameplayStatics::ApplyDamage(PlayerGameState->Player, Damage, GetController(), this, nullptr);
}

void AEnemyBoss::HookEndEvent()
{
	bIsHookBegan = false;
	bIsHookPull = true;
}

void AEnemyBoss::UseShockBlast()
{
	if (bShockBlastReady && !bSkillUsing)
	{
		bSkillUsing = true;
		bShockBlastReady = false;
		ShockBlastCooldown = DesiredBlastCooldown;
		PlayAnimMontage(ShockBlastAnim);
		GetCharacterMovement()->StopMovementImmediately();
	}
}

void AEnemyBoss::UseSkill()
{
	ResetAttack();
	UseHook();
	UseShockBlast();
	UseStomachGun();
	UseEarthquake();
}

void AEnemyBoss::SaveAttack()
{
	const float SelectedRange = UKismetMathLibrary::SelectFloat(Range * 1.2f, Range, PlayerGameState->Triggered);
	if (!IsDead && PlayerGameState->GetTarget() && DistanceToTarget < SelectedRange)
	{
		AttackTrigger();
	}
}

void AEnemyBoss::ResetAttack()
{
	AttackCount = 0;
	bIsAttacking = false;
	bInRange = false;
}

void AEnemyBoss::ResetSkills()
{
	bIsHookBegan = false;
	bIsHookPull = false;
	bSkillUsing = false;
}

void AEnemyBoss::UseStomachGun()
{
	if (bStomachGunReady && !bSkillUsing)
	{
		bSkillUsing = true;
		bStomachGunReady = false;
		StomachGunCooldown = DesiredStomachGunCooldown;
		GetCharacterMovement()->StopMovementImmediately();
		PlayAnimMontage(StomachGunMontage);
	}
}

void AEnemyBoss::UseEarthquake()
{
	if (bEarthquakeReady && !bSkillUsing)
	{
		bSkillUsing = true;
		bEarthquakeReady = false;
		EarthquakeCooldown = DesiredEarthquakeCooldown;
		GetCharacterMovement()->StopMovementImmediately();
		PlayAnimMontage(EarthquakeMontage);
	}
}

