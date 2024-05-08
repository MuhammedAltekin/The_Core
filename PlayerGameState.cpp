// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerGameState.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

void APlayerGameState::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerH;
	OnceTimer(TimerH, Timer(); , true, 0.1f, 0);
}

void APlayerGameState::GetEnemyDamageMultiply_Implementation(float& Value)
{
	IGameStateInterface::GetEnemyDamageMultiply_Implementation(Value);
	Value = EnemyDamageMultiply;
}

void APlayerGameState::GetSpawnRateMultiply_Implementation(float& Value)
{
	IGameStateInterface::GetSpawnRateMultiply_Implementation(Value);
	Value = EnemySpawnRate;
}

void APlayerGameState::GetGameTimeMultiply_Implementation(float& Value)
{
	IGameStateInterface::GetGameTimeMultiply_Implementation(Value);
	Value = GameTimeMultiply;
}

void APlayerGameState::GetBombDamage_Implementation(float& Damage)
{
	IGameStateInterface::GetBombDamage_Implementation(Damage);

	switch (BombLevel)
	{
	case 0:
		Damage = 0;
		break;
	case 1:
		Damage = 1000;
		break;
	case 2:
		Damage = 2000;
		break;
	case 3:
		Damage = 3000;
		break;
	case 4:
		Damage = 4000;
		break;
	default:
		Damage = 0;
	}
}

float APlayerGameState::GetShotgunRadius() const
{
	switch (ShotgunShellFocusingLevel)
	{
	case 0:
		return 15;
	case 1:
		return  12.5f;
	case 2:
		return  10;
	case 3:
		return  7.5f;
	case 4:
		return  5;
	default:
		return 15;
		
	}
}

float APlayerGameState::GetLaserDamage()
{
	switch (LaserDamageLevel)
	{
	case 0:
		return 400;
	case 1:
		return 550;
	case 2:
		return 700;
	case 3:
		return 850;
	case 4:
		return 1000;
	default:
		return 350;
	}
}

void APlayerGameState::GetExplosiveBulletLevel_Implementation(int& Level)
{
	IGameStateInterface::GetExplosiveBulletLevel_Implementation(Level);

	Level = ExplosiveBulletLevel;
}

void APlayerGameState::GetBulletBounceCount_Implementation(int& BounceCount)
{
	IGameStateInterface::GetBulletBounceCount_Implementation(BounceCount);

	BounceCount = BulletBounceLevel;
}

void APlayerGameState::GetRangedBullet_Implementation(float& DamageValue)
{
	IGameStateInterface::GetRangedBullet_Implementation(DamageValue);

	DamageValue = RangedBulletDamage;
}


void APlayerGameState::GetLongRangedCannonDamage_Implementation(float& DamageValue)
{
	DamageValue = LongRangedCannonDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetLongRangedBulletDamage_Implementation(float& DamageValue)
{
	DamageValue = LongRangedBulletDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetBomberDamage_Implementation(float& DamageValue)
{
	DamageValue = BomberDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetBufferDamage_Implementation(float& Damage)
{
	Damage = BufferDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetMeleeDamage_Implementation(float& DamageValue)
{
	IGameStateInterface::GetMeleeDamage_Implementation(DamageValue);

	DamageValue = MeleeDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetTankAttackDamage_Implementation(float& DamageValue)
{
	IGameStateInterface::GetTankAttackDamage_Implementation(DamageValue);
	DamageValue = TankAttackDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetTankSkillDamage_Implementation(float& DamageValue)
{
	IGameStateInterface::GetTankSkillDamage_Implementation(DamageValue);
	DamageValue = TankSkillDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetBossAttackDamage_Implementation(float& DamageValue)
{
	IGameStateInterface::GetBossAttackDamage_Implementation(DamageValue);
	DamageValue = BossAttackDamage * EnemyDamageMultiply;
}

float APlayerGameState::GetBossBlastDamage_Implementation()
{
	return  BossBlastDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetBossEarthQuakeDamage_Implementation(float& DamageValue)
{
	IGameStateInterface::GetBossEarthQuakeDamage_Implementation(DamageValue);
	DamageValue = BossEarthQuakeDamage * EnemyDamageMultiply;
}

float APlayerGameState::GetBossHookDamage_Implementation()
{
	return BossHookDamage * EnemyDamageMultiply;
}

void APlayerGameState::GetBossStomachDamage_Implementation(float& DamageValue)
{
	IGameStateInterface::GetBossStomachDamage_Implementation(DamageValue);
	DamageValue = BossStomachDamage * EnemyDamageMultiply;
}

const AActor* APlayerGameState::GetCurrentTarget_Implementation()
{
	return GetTarget();
}

bool APlayerGameState::GetTriggered()
{
	return Triggered;
}

int APlayerGameState::GetSetEnemyAmount(EEnemies EnemyType, bool IsGet, bool IsWithout, bool IsAdd, EEnemies WithoutType)
{
	int SelectInt = 0;
	int WithoutInt = 0;
	switch (EnemyType)
	{
	case 0:
		SelectInt = BomberAmount;
		break;
	case 1:
		SelectInt = LongRangedAmount;
		break;
	case 2:
		SelectInt = BossAmount;
		break;
	case 3:
		SelectInt = MeleeAmount;
		break;
	case 4:
		SelectInt = RangedAmount;
		break;
	case 5:
		SelectInt = TankAmount;
		break;
	case 6:
		SelectInt = BufferAmount;
		break;
	case 7:
		SelectInt = EnemyAmount;
		break;
	default:;
	}
	switch (WithoutType)
	{
	case 0:
		WithoutInt = BomberAmount;
		break;
	case 1:
		WithoutInt = LongRangedAmount;
		break;
	case 2:
		WithoutInt = BossAmount;
		break;
	case 3:
		WithoutInt = MeleeAmount;
		break;
	case 4:
		WithoutInt = RangedAmount;
		break;
	case 5:
		WithoutInt = TankAmount;
		break;
	case 6:
		WithoutInt = BufferAmount;
		break;
	case 7:
		WithoutInt = EnemyAmount;
		break;
	default:;
	}
	if (IsGet)
	{
		return UKismetMathLibrary::SelectInt(SelectInt - WithoutInt, SelectInt, IsWithout);
	}
	else
	{
		if (IsAdd)
		{
			SelectInt++;
			EnemyAmount++;
			switch (EnemyType)
			{
			case 0:
				BomberAmount = SelectInt;
				break;
			case 1:
				LongRangedAmount = SelectInt;
				break;
			case 2:
				BossAmount = SelectInt;
				break;
			case 3:
				MeleeAmount = SelectInt;
				break;
			case 4:
				RangedAmount = SelectInt;
				break;
			case 5:
				TankAmount = SelectInt;
				break;
			case 6:
				BufferAmount = SelectInt;
				break;
			case 7:
				EnemyAmount = SelectInt;
				break;
			default:;
			}
			return SelectInt;
		}
		else
		{
			SelectInt--;
			EnemyAmount--;
			switch (EnemyType)
			{
			case 0:
				BomberAmount = SelectInt;
				break;
			case 1:
				LongRangedAmount = SelectInt;
				break;
			case 2:
				BossAmount = SelectInt;
				break;
			case 3:
				MeleeAmount = SelectInt;
				break;
			case 4:
				RangedAmount = SelectInt;
				break;
			case 5:
				TankAmount = SelectInt;
				break;
			case 6:
				BufferAmount = SelectInt;
				break;
			case 7:
				EnemyAmount = SelectInt;
				break;
			default:;
			}
			return SelectInt;
		}

	}
}

float APlayerGameState::GetGetPlayerGunDamageLevel_Implementation()
{
	return PlayerGunDamageUpgradeLevel;
}

void APlayerGameState::SetMouseSensitivity_Implementation(float Sensitivity)
{
	IGameStateInterface::SetMouseSensitivity_Implementation(Sensitivity);

	MouseSensitivity = Sensitivity;
}

void APlayerGameState::GetMouseSensitivity_Implementation(float& Sensitivity)
{
	IGameStateInterface::GetMouseSensitivity_Implementation(Sensitivity);

	Sensitivity = MouseSensitivity;
}


void APlayerGameState::Timer()
{
	if (RemainingNanocells > 0 || BaseDestroyed)
	{
		RemainingNanocells -= 0.1f;
		Triggered = false;
	}
	else
	{
		Triggered = !IgnoreBase;
		RemainingNanocells = 0;
	}
}

void APlayerGameState::ClearWholeTimers()
{
	for (int i = 0; i < WholeTimers.Num(); i++)
	{
		GetWorldTimerManager().ClearTimer(WholeTimers[i]);
	}
	GetWorldTimerManager().ClearAllTimersForObject(this);
	GetWorldTimerManager().ClearAllTimersForObject(Player);
}

AActor* APlayerGameState::GetTarget()
{
	if (Triggered)
	{
		return Center;
	}
	else
	{
		return Player;
	}
}

void APlayerGameState::CheckLevelUp()
{
	if (PlayerExp >= ExpToLevelUp)
	{
		PlayerLevel++;
		UnusedPoints++;
		PlayerExp = (PlayerExp - ExpToLevelUp);
		ExpToLevelUp *= LevelMultiply;
		CheckLevelUp();
		LevelUpEventBP();
		UGameplayStatics::SpawnEmitterAttached(LevelUpVFX,Player->GetMesh());
		UGameplayStatics::SpawnSound2D(GetWorld(),LevelUpSound);
	}
}
