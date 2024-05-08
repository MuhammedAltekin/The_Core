// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "PlayerGameState.h"
#include "SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "Spawner.generated.h"

UCLASS()
class ALEYIKAPAKPROJESI_API ASpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetSpawnPointArrays();
	UFUNCTION(BlueprintImplementableEvent)
	void BossSpawn();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnEnemyInBP();
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnSupportInBP();
	UFUNCTION(BlueprintImplementableEvent)
	void GetRandomSpawnPoint(bool Big);
	UFUNCTION(BlueprintCallable)
	void BeginSpawn();
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool ChooseSpawnClass(float StartPercentage, float EndPercentage) const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTransform ChooseSpawnPoint(bool Big);

	void SpawnEnemy();
	void SpawnSupport();


	UPROPERTY()
	APlayerGameState* PlayerGameState;
	UPROPERTY(EditAnywhere)
	bool bCanProcess;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float WaveFrequency;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int MaxGameTime;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int BossArrivalTime;
	UPROPERTY(EditAnywhere)
	int SupportSpawnFrequency;
	UPROPERTY(BlueprintReadWrite)
	TArray<ASpawnPoint*> SpawnPoints;
	UPROPERTY(BlueprintReadWrite)
	TArray<ASpawnPoint*> BigSpawnPoints;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<TSubclassOf<AEnemyBase>> Supports;
	UPROPERTY(BlueprintReadWrite)
	ASpawnPoint* RandomPoint;
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle OnceTimerHand;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* SpawnFX;
	bool bEndGame;
	int SupportSpawnCounter;
	FTimerHandle GameTimeTh;
};
