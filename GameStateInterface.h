// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enemies.h"
#include "GameStateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameStateInterface : public UInterface
{
	GENERATED_BODY()

};

/**
 * 
 */
class ALEYIKAPAKPROJESI_API IGameStateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int GetSetEnemyAmount(EEnemies EnemyType, bool IsGet, bool IsWithout, bool IsAdd, EEnemies WithoutType) = 0;

	//Enemy
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetRangedBullet(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetLongRangedCannonDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetLongRangedBulletDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetBomberDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetBufferDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetMeleeDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetTankAttackDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetTankSkillDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetBossAttackDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	float GetBossBlastDamage();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetBossStomachDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetBossEarthQuakeDamage(float& DamageValue);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	float GetBossHookDamage();

	//Player
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	float GetGetPlayerGunDamageLevel();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetBulletBounceCount(int& BounceCount);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetExplosiveBulletLevel(int& Level);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetBombDamage(float& Damage);
	

	//General
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	const AActor* GetCurrentTarget();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetEnemyDamageMultiply(float& Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetSpawnRateMultiply(float& Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetGameTimeMultiply(float& Value);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void SetMouseSensitivity(float Sensitivity);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BPI Game State")
	void GetMouseSensitivity(float& Sensitivity);

	virtual bool GetTriggered() = 0;


};
