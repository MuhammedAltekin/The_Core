// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyBoss.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API AEnemyBoss : public AEnemyBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	//FUNCTIONS
	UFUNCTION(BlueprintImplementableEvent)
	void MoveToLoc();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintImplementableEvent)
	void AttackTrigger();
	UFUNCTION(BlueprintCallable)
	void SaveAttack();
	UFUNCTION(BlueprintCallable)
	void ResetAttack();
	UFUNCTION(BlueprintCallable)
	void ResetSkills();

	void UseSkill();
	void CooldownCounting();


	//Variables
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	bool bSkillUsing;
	UPROPERTY(VisibleAnywhere)
	bool bSkillReady;

	FTimerHandle OnceTHandle;


	//Hook
	void UseHook();
	UFUNCTION(BlueprintCallable)
	void HookBeginEvent();
	UFUNCTION(BlueprintCallable)
	void HookPullEvent();
	UFUNCTION(BlueprintCallable)
	void HookPullBlast(float PushForce, float Damage);
	UFUNCTION(BlueprintCallable)
	void HookEndEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void PullImplementEvent(FVector OutPullForce);

	UPROPERTY(BlueprintReadWrite)
	bool bIsHookBegan;
	UPROPERTY(BlueprintReadWrite)
	bool bIsHookPull;
	UPROPERTY(VisibleAnywhere)
	bool bHookReady;
	UPROPERTY(EditDefaultsOnly)
	float HookCooldown;
	UPROPERTY(EditDefaultsOnly)
	float DesiredHookCooldown;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HookBeginAnim;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* HookPullAnim;

	//Attack
	bool bIsAttacking;
	UPROPERTY(BlueprintReadWrite)
	bool bInRange;
	UPROPERTY(BlueprintReadWrite)
	int  AttackCount;

	//ShockBlast
	void UseShockBlast();

	UPROPERTY(VisibleAnywhere)
	bool bShockBlastReady;
	UPROPERTY(EditDefaultsOnly)
	float ShockBlastCooldown;
	UPROPERTY(EditDefaultsOnly)
	float DesiredBlastCooldown;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShockBlastAnim;
	

	//X Ýmpact Variable
	UPROPERTY(BlueprintReadWrite)
	bool bXImpactReady;

	float XImpactCooldown;


	//StomachGun
	UFUNCTION()
	void UseStomachGun();

	UPROPERTY(VisibleAnywhere)
	bool bStomachGunReady;
	UPROPERTY(EditDefaultsOnly)
	float StomachGunCooldown;
	UPROPERTY(EditDefaultsOnly)
	float DesiredStomachGunCooldown;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* StomachGunMontage;


	//Earthquake
	UFUNCTION()
	void UseEarthquake();

	UPROPERTY(VisibleAnywhere)
	bool bEarthquakeReady;
	UPROPERTY(EditDefaultsOnly)
	float EarthquakeCooldown;
	UPROPERTY(EditDefaultsOnly)
	float DesiredEarthquakeCooldown;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EarthquakeMontage;
};
