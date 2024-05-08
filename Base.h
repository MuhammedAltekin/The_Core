// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Base.generated.h"

class APlayerGameState;

UCLASS()
class ALEYIKAPAKPROJESI_API ABase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//CLASS COMPONENTS
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Shield;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Center;
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Sphere;
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* Root;
	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* MaterialInterfaceParameter;
	UPROPERTY()
	UMaterialInstanceDynamic* ShieldMaterial;

	//FUNCTIONS
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void InBattleRegenerates();
	void SettingMaterialParameters() const;

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyImplementEvent();

	//Class Variable
	UPROPERTY()
	APlayerGameState* PlayerGameState;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float CurrentShield;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxShield;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditDefaultsOnly)
	float ShieldRegen;
	UPROPERTY(EditDefaultsOnly)
	float HealthRegen;

	bool Destroyed;
	float InBattleTime;
	FTimerHandle OnceTHandle;

	

};
