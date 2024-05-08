// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootNotify.h"

#include "PlayerGameState.h"
#include "Kismet/KismetMathLibrary.h"

void UShootNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// ReSharper disable once CppDeprecatedEntity
	Super::Notify(MeshComp, Animation);

	if (APlayerGameState* PlayerGameState = Cast<APlayerGameState>(GetWorld()->GetGameState()))
	{
		FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MeshComp->GetSocketLocation(InSocketName), PlayerGameState->GetTarget()->GetActorLocation());
		GetWorld()->SpawnActor<AActor>(SpawningActor, MeshComp->GetSocketLocation(InSocketName),LookAtRot);
	}
	
}
