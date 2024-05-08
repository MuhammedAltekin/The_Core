// Fill out your copyright notice in the Description page of Project Settings.


#include "Base.h"

#include "PlayerGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define String(Text) FString(TEXT(Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

// Sets default values
ABase::ABase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<UArrowComponent>(FName("Root"));
	Root->SetupAttachment(RootComponent);

	Shield = CreateDefaultSubobject<UStaticMeshComponent>(FName("Shield"));
	Shield->SetupAttachment(Root);

	Center = CreateDefaultSubobject<UStaticMeshComponent>(FName("Center"));
	Center->SetupAttachment(Root);

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(FName("Sphere"));
	Sphere->SetupAttachment(Center);
}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();

	PlayerGameState = Cast<APlayerGameState>(UGameplayStatics::GetGameState(GetWorld()));
	PlayerGameState->Center = this;

	ShieldMaterial = Shield->CreateDynamicMaterialInstance(0,MaterialInterfaceParameter,FName("None"));

	
	OnceTimer(OnceTHandle,
		InBattleRegenerates();, true, .1f, 0);
	PlayerGameState->WholeTimers.Add(OnceTHandle);
}

// Called every frame
void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SettingMaterialParameters();
}

float ABase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	InBattleTime = 0.5f;
	if (CurrentShield <= 0)
	{
		if (CurrentHealth <= 0)
		{
			PlayerGameState->BaseDestroyed = true;
			Destroyed = true;
			PlayerGameState->Player->DeathEvent();
			DestroyImplementEvent();
		}
		else
		{
			CurrentHealth -= DamageAmount;
		}
	}
	else
	{
		CurrentShield -= DamageAmount;
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	

}

void ABase::InBattleRegenerates()
{
	if (InBattleTime <= 0)
	{
		if (CurrentShield < MaxShield)
		{
			CurrentShield += ShieldRegen * .1f;
		}
		else
		{
			if (CurrentHealth < MaxHealth)
			{
				CurrentHealth += HealthRegen * .1f;
			}
		}
	}
	else
	{
		InBattleTime -= .1f;
	}
}

void ABase::SettingMaterialParameters() const
{
	if (!(CurrentShield >= MaxShield))
	{
		const float ClampedParameterValue = UKismetMathLibrary::MapRangeClamped(CurrentShield, 0, MaxShield, .1f, 1);
		if (ShieldMaterial && !Destroyed)
		{
			ShieldMaterial->SetScalarParameterValue(FName("Opacity"), ClampedParameterValue);
			ShieldMaterial->SetScalarParameterValue(FName("GlowAmount"), ClampedParameterValue * 10);
		}
	}
}

