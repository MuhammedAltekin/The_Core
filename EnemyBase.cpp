// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "AIController.h"
#include "DamageNumbersActor.h"
#include "NiagaraFunctionLibrary.h"
#include "AI/NavigationSystemBase.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define String(Text) FString(TEXT(Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

// Sets default values
// ReSharper disable once CppPossiblyUninitializedMember
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	bUseControllerRotationYaw = false;

	PlayerGameState = Cast<APlayerGameState>(UGameplayStatics::GetGameState(GetWorld()));
	IGameState = Cast<IGameStateInterface>(PlayerGameState);

	if (IGameState)
		IGameState->GetSetEnemyAmount(EnemyType, false, false, true, EEnemies::Bomber);

	WingAudio = UGameplayStatics::SpawnSoundAttached(WingSound,RootComponent);

	if (bSupportMode)
		ShieldAudio = UGameplayStatics::SpawnSoundAttached(ShieldSound, RootComponent);

	SpawnDefaultController();	
	AIController = Cast<AAIController>(GetController());

	if (PlayerGameState)
	{
		Health *= PlayerGameState->EnemyHealthMultiply;
		DroppedExperience *= PlayerGameState->EnemyHealthMultiply;	
		if (PlayerGameState)
		{
			const float BonusPower = PlayerGameState->EnemyArmorMultiply * (PlayerGameState->GameTime / (15 / PlayerGameState->GameTimeMultiply)); 
			if (Armor + BonusPower > 80)
			{
				Armor += BonusPower;
				const float RemainingBonus = Armor - 80;
				Armor -= RemainingBonus;
				Health += RemainingBonus * 100;
			}
			else
			{
				Armor += BonusPower;
			}
		}
	}
	
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Speed = GetVelocity().Length();

	if (TurnToTarget)
	{
		RotatingToTarget();
	}

	if (PlayerGameState->GetTarget())
	{
		DistanceToTarget = UKismetMathLibrary::Vector_Distance(PlayerGameState->GetTarget()->GetActorLocation(), GetActorLocation());
	}
}


// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyBase::DeathEvent()
{
	//Spawn Capsule
	if (UKismetMathLibrary::RandomBoolWithWeight(CapsuleDropPossibility / 100))
	{
		GetWorld()->SpawnActor<ANanocellCapsule>(NanocellCapsuleActor, FVector(GetActorLocation().X, GetActorLocation().Y, 0), FRotator(0));
	}
	//Drop Exo
	if (PlayerGameState)
	{
		PlayerGameState->PlayerExp += DroppedExperience;
		PlayerGameState->CheckLevelUp();
	}
	//Death
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathDialogSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAttached(DeathFX, GetMesh(), FName(""), FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, true, true);
	GetMesh()->SetVisibility(false);
	SetActorEnableCollision(ECollisionEnabled::NoCollision);
	if (ShieldAudio)
		ShieldAudio->Stop();
	if (WingAudio)
		WingAudio->Stop();
	GetCharacterMovement()->StopMovementImmediately();
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DeathSound, GetActorLocation());
	IsDead = true;
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	PlayerGameState->KillCount++;
	PlayerGameState->GetSetEnemyAmount(EnemyType, false, false, false, EEnemies::All_Enemy);
	OnceTimer(DeadTh, Destroy();, false, 1, 2);
	PlayerGameState->WholeTimers.Add(DeadTh);
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	if (EventInstigator == UGameplayStatics::GetPlayerController(GetWorld(),0))
	{
		const float ArmorReduction = UKismetMathLibrary::FClamp(DamageAmount - (DamageAmount / 100 * Armor), 0, 100000);
		Health -= ArmorReduction;
		GetWorld()->SpawnActor<ADamageNumbersActor>(DamageNumbersActor, UKismetMathLibrary::SelectVector(GetActorLocation(),IncomingDamageLocation,UKismetMathLibrary::EqualEqual_VectorVector(IncomingDamageLocation,FVector(0), 0)), FRotator(0))->Damage = ArmorReduction;
		if (Health <= 0 && !IsDead)
		{
			DeathEvent();
		}
		
	}

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEnemyBase::TurnTarget()
{
	if (PlayerGameState->GetTarget())
	{
		const FVector TargetLoc = UKismetMathLibrary::SelectVector(PlayerGameState->Player->GetActorLocation(), PlayerGameState->GetTarget()->GetActorLocation(), bSupportMode);;
		const FRotator EaseB = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLoc);
		const FRotator Rotation = UKismetMathLibrary::REase(GetActorRotation(), EaseB,.1f,true,EEasingFunc::Linear,2,2);
		SetActorRotation(FRotator(0,Rotation.Yaw,0));
	}
}

void AEnemyBase::RotatingToTarget()
{
	if (!IsRotating)
	{
		IsRotating = true;
		TurnTarget();
		const float Delay = GetWorld()->GetDeltaSeconds() / UGameplayStatics::GetGlobalTimeDilation(GetWorld());
		OnceTimer(RotatingTh,
			IsRotating = false;
			, false, 1, Delay);
	}
}



