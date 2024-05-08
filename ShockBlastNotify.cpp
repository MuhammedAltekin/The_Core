// Fill out your copyright notice in the Description page of Project Settings.


#include "ShockBlastNotify.h"

#include "EnemyBoss.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UShockBlastNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// ReSharper disable once CppDeprecatedEntity
	Super::Notify(MeshComp, Animation);

	const TArray<AActor*> IgnoredActors;
	static FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), MeshComp->GetSocketLocation(FName("")), MeshComp->GetSocketLocation(FName("")), Radius, TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);

	if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == HitResult.GetActor())
	{
		if (AEnemyBoss* EnemyBoss = Cast<AEnemyBoss>(MeshComp->GetOwner()))
		{
		EnemyBoss->HookPullBlast(PushForce,EnemyBoss->PlayerGameState->GetBossBlastDamage_Implementation());
		}
	}

}
