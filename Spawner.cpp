// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define String(Text) FString(TEXT(Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)


// Sets default values
ASpawner::ASpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}


// Called when the game starts or when spawned
void ASpawner::BeginPlay()
{
	Super::BeginPlay();
	if (bCanProcess)
	{
		PlayerGameState = Cast<APlayerGameState>(UGameplayStatics::GetGameState(GetWorld()));
		SetSpawnPointArrays();
		OnceTimer(GameTimeTh,
			PlayerGameState->GameTime++;
			if (PlayerGameState->GameTime >= MaxGameTime)
			{
				PlayerGameState->bVictory = true;
				PlayerGameState->Player->DeathEvent();
			}
		, true, 1, 1);
		PlayerGameState->WholeTimers.Add(GameTimeTh);
	}
}

// Called every frame
void ASpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawner::SpawnEnemy()
{
		if (ChooseSpawnClass(90, 50))
		{
			SpawnEnemyInBP();
		}
}

void ASpawner::SpawnSupport()
{
	if (SupportSpawnCounter == SupportSpawnFrequency)
	{
		SupportSpawnCounter = 0;
		SpawnSupportInBP();
	}
	else
	{
		SupportSpawnCounter++;
	}
}

void ASpawner::BeginSpawn()
{
	if (bCanProcess)
	{
		if (!PlayerGameState->WaveStatus)
		{
			PlayerGameState->WaveStatus = true;
			SpawnEnemy();

			if (PlayerGameState->GameTime >= BossArrivalTime)
			{
				BossSpawn();
			}
			else
			{
				SpawnSupport();
			}

			FTimerHandle OnceTh;
			OnceTimer(OnceTh,
				PlayerGameState->WaveStatus = false;
			, false, WaveFrequency, 0);
		}
	}
}

bool ASpawner::ChooseSpawnClass(const float StartPercentage, const float EndPercentage) const
{
	const float ClampedValue = UKismetMathLibrary::MapRangeClamped(PlayerGameState->GameTime, 0, MaxGameTime, StartPercentage / 100, EndPercentage / 100);
	return UKismetMathLibrary::RandomBoolWithWeight(ClampedValue);
}

FTransform ASpawner::ChooseSpawnPoint(bool Big)
{
	GetRandomSpawnPoint(Big);
	if (RandomPoint->Usable)
	{
		RandomPoint->Reusable();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnFX, RandomPoint->GetActorTransform().GetLocation(), FRotator(0), FVector(10));
	}
	else
	{
		BeginSpawn();
	}
	return RandomPoint->GetActorTransform();
}
