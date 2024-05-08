// Fill out your copyright notice in the Description page of Project Settings.


#include "HookCollisionNotifyState.h"

#include "EnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))

void UHookCollisionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,float FrameDeltaTime)
{
	// ReSharper disable once CppDeprecatedEntity
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	AEnemyBoss* Boss = Cast<AEnemyBoss>(MeshComp->GetOwner());
	
	const TArray<AActor*> IgnoredActors;
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(MeshComp, MeshComp->GetSocketLocation(FName("chain_tip_r")), MeshComp->GetSocketLocation(FName("chain_tip_r")), 80, TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);
	
	if (HitResult.GetActor() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (Boss)
		{
			Boss->HookPullEvent();
		}
		
	}
	else
	{
		if (Boss)
		{
			Boss->HookEndEvent();
		}
		
	}
	
			
}
