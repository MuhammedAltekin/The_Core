// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunType.h"
#include "InputAction.h"
#include "PlayerCharacterInterface.h"
#include "GameFramework/Character.h"
#include "Solider.generated.h"

class UInputMappingContext;

UCLASS(Blueprintable)
class ALEYIKAPAKPROJESI_API ASolider : public ACharacter , public IPlayerCharacterInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASolider();

protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


public:

	/** Returns CameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	//Interface
	virtual float GetBulletDamage_Implementation() override;
	virtual void IncreaseHealthArmor_Implementation(float HealthValue, float ArmorValue) override;


	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//FUNCTIONS
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dash_Started();
	void DashEvent(float DashValue, float Cooldown);
	FVector FindTargetLoc();
	float EaseRotation(const FRotator& EaseB) const;
	float GetSpeed() const;
	float CalcTurnRate() const;
	double CalcEaseRotation();
	double CalcCachedRot();
	void CooldownCounting();
	FVector CalcDashLocation(float DashMultiplier) const;
	void TickTimerEvent();
	void LMB_Triggered();
	void KnockbackBegin();
	void KnockbackEnd();
	void SlowMotion_Started();
	void SlowMotionEvent();

	UFUNCTION(BlueprintCallable)
	void ScopeOpen();
	UFUNCTION(BlueprintCallable)
	void ScopeClose();
	UFUNCTION(BlueprintCallable)
	void ShootingEvent(FVector TargetLocation, EGunType Gun);
	UFUNCTION(BlueprintImplementableEvent)
	void LaserShootBP(FVector TargetLocat);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DeathEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySlowMoTalk();
	//Variable
		//Input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LMB_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RMB_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LShift_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* KeyF_Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* Look_Action;

		//General
	UPROPERTY()
	APlayerController* PlayerController;
	UPROPERTY(BlueprintReadWrite)
	class APlayerGameState* PlayerGameState;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool TPSMod = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool Scope;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool Rotating;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool ShootingTriggered;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool CanShoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool SlowMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	bool Knockback;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	bool Stun;
	UPROPERTY(BlueprintReadWrite)
	bool AimOnTarget;
	UPROPERTY(BlueprintReadWrite)
	bool SkillUsing;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool isDead;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float TurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float CachedYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float CurrentYaw;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float BulletDamage = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	float Health = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float Armor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float MaxHealth = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float DashDistance = 150;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	float CD_SlowMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variable)
	float PlayerShootDensityMultiply;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Variable)
	float GunShootDensity = 1;
	UPROPERTY(BlueprintReadWrite)
	float LaserRadius;
	UPROPERTY(EditDefaultsOnly)
	float LaserDamage;
	UPROPERTY(EditAnywhere)
	float ShotgunBulletCount;
	UPROPERTY(EditAnywhere)
	float ShotgunRadius;
	UPROPERTY(BlueprintReadWrite)
	float CD_Bomb;
	UPROPERTY(BlueprintReadWrite)
	float CD_Ultimate;
	UPROPERTY(BlueprintReadWrite)
	float CD_Dash;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	FRotator VelocityDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	UParticleSystem* FXDashStart;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	UParticleSystem* FXDashEnd;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Sound_TeleportStart;
	UPROPERTY(EditDefaultsOnly)
	USoundBase* Sound_TeleportEnd;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	USoundBase* SFXGunShoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Variable)
	UClass* PlayerGunBullet;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TEnumAsByte<EGunType> GunType;
	UPROPERTY(BlueprintReadWrite)
	TArray<FHitResult> LaserHitResults;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> LaserHitLocations;
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> LaserHitActors;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* LaserHitEnemyVFX;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageType> LaserTraceChanel;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ShotgunShootVFX;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ShotgunBullet;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ShotgunMontage;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> AssaultShake;
	
	bool bTurnCache;
	bool ShootingCache;
	bool bSlowMotionBegin;
	float DesiredFov = 110;
	float SlowMo_Multiplier;
	float DesiredTimeDilation;
	FTimerHandle GunShootingTh;
	FTimerHandle TurnCacheTh;
	FTimerHandle CdTh;
	
	

};
