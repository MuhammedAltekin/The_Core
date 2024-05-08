// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base.h"
#include "GameStateInterface.h"
#include "Solider.h"
#include "GameFramework/GameStateBase.h"
#include "PlayerGameState.generated.h"
/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API APlayerGameState : public AGameStateBase , public IGameStateInterface
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//Interface Functions
		//Player
	virtual void GetEnemyDamageMultiply_Implementation(float& Value) override;
	virtual void GetExplosiveBulletLevel_Implementation(int& Level) override;
	virtual void GetBulletBounceCount_Implementation(int& BounceCount) override;
	virtual void GetBombDamage_Implementation(float& Damage) override;
	virtual float GetGetPlayerGunDamageLevel_Implementation() override;

		//General
	virtual void GetSpawnRateMultiply_Implementation(float& Value) override;
	virtual void GetGameTimeMultiply_Implementation(float& Value) override;
	virtual const AActor* GetCurrentTarget_Implementation() override;
	virtual bool GetTriggered() override;
	virtual int GetSetEnemyAmount(EEnemies EnemyType, bool IsGet, bool IsWithout, bool IsAdd, EEnemies WithoutType) override;
	virtual void SetMouseSensitivity_Implementation(float Sensitivity) override;
	virtual void GetMouseSensitivity_Implementation(float& Sensitivity) override;

		//Enemy
	virtual void GetRangedBullet_Implementation(float& DamageValue) override;
	virtual void GetLongRangedCannonDamage_Implementation(float& DamageValue) override;
	virtual void GetLongRangedBulletDamage_Implementation(float& DamageValue) override;
	virtual void GetBomberDamage_Implementation(float& DamageValue) override;
	virtual void GetBufferDamage_Implementation(float& Damage ) override;
	virtual void GetMeleeDamage_Implementation(float& DamageValue) override;
	virtual void GetTankAttackDamage_Implementation(float& DamageValue) override;
	virtual void GetTankSkillDamage_Implementation(float& DamageValue) override;
	virtual void GetBossAttackDamage_Implementation(float& DamageValue) override;
	virtual float GetBossBlastDamage_Implementation() ;
	virtual void GetBossEarthQuakeDamage_Implementation(float& DamageValue) override;
	virtual float GetBossHookDamage_Implementation() ;
	virtual void GetBossStomachDamage_Implementation(float& DamageValue) override;

	
	//FUNCTION
	UFUNCTION(BlueprintCallable,BlueprintPure)
	AActor* GetTarget();
	UFUNCTION(BlueprintImplementableEvent)
	void LevelUpEventBP();
	UFUNCTION(BlueprintCallable,BlueprintPure)
	float GetLaserDamage();
	UFUNCTION(BlueprintCallable)
	void CheckLevelUp();
	UFUNCTION(BlueprintCallable)
	void ClearWholeTimers();

	float GetShotgunRadius() const;
	void Timer();

	//Variable
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* LevelUpVFX;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* LevelUpSound;


	//Game Information's
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ABase* Center;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASolider* Player;
	UPROPERTY(VisibleAnywhere)
	bool BaseDestroyed;
	UPROPERTY(EditAnywhere)
	bool IgnoreBase;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool FPS_Display;
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	TArray<FTimerHandle> WholeTimers;


	//Game Status Information's
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Triggered;
	UPROPERTY(BlueprintReadWrite)
	bool bVictory;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bEndGame;
	UPROPERTY(BlueprintReadWrite)
	float RemainingNanocells = 10;
	UPROPERTY(BlueprintReadWrite)
	int GameTime;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int KillCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EnemySpawnRate = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EnemyDamageMultiply = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EnemyHealthMultiply = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float EnemyArmorMultiply = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float GameTimeMultiply = 1;

	bool WaveStatus;


	//Enemy Attack Damages
	UPROPERTY(EditDefaultsOnly)
	float BufferDamage;
	UPROPERTY(EditDefaultsOnly)
	float BomberDamage;
	UPROPERTY(EditDefaultsOnly)
	float RangedBulletDamage;
	UPROPERTY(EditDefaultsOnly)
	float LongRangedCannonDamage;
	UPROPERTY(EditDefaultsOnly)
	float LongRangedBulletDamage;
	UPROPERTY(EditDefaultsOnly)
	float MeleeDamage;
	UPROPERTY(EditDefaultsOnly)
	float TankAttackDamage;
	UPROPERTY(EditDefaultsOnly)
	float TankSkillDamage;
	UPROPERTY(EditDefaultsOnly)
	float BossAttackDamage;
	UPROPERTY(EditDefaultsOnly)
	float BossHookDamage;
	UPROPERTY(EditDefaultsOnly)
	float BossStomachDamage;
	UPROPERTY(EditDefaultsOnly)
	float BossBlastDamage;
	UPROPERTY(EditDefaultsOnly)
	float BossEarthQuakeDamage;

	//Enemy Counts
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int EnemyAmount;
	UPROPERTY(VisibleAnywhere)
	int BossAmount;
	UPROPERTY(VisibleAnywhere)
	int MeleeAmount;
	UPROPERTY(VisibleAnywhere)
	int RangedAmount;
	UPROPERTY(VisibleAnywhere)
	int LongRangedAmount;
	UPROPERTY(VisibleAnywhere)
	int TankAmount;
	UPROPERTY(VisibleAnywhere)
	int BomberAmount;
	UPROPERTY(VisibleAnywhere)
	int BufferAmount;

	//Player & Skills Information
	UPROPERTY(BlueprintReadWrite)
	bool LevelingProcess;
	UPROPERTY(BlueprintReadWrite)
	int PlayerLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UnusedPoints;
	UPROPERTY(BlueprintReadWrite)
	float PlayerExp;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float MouseSensitivity = 1;
	UPROPERTY(EditAnywhere)
	float LevelMultiply = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float ExpToLevelUp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bAssaultRifle;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bLaserGun;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bShotgun;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int SlowMotionLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int DashLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int BombLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UltimateLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int BulletBounceLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ExplosiveBulletLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int LaserDamageLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int LaserFireRateLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ShotgunShellFocusingLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ShotgunFireRateLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int PlayerGunDamageUpgradeLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int PlayerHealthUpgradeLevel;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int PlayerArmorUpgradeLevel;
};
