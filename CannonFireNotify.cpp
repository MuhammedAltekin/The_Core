// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonFireNotify.h"

void UCannonFireNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	// ReSharper disable once CppDeprecatedEntity
	Super::Notify(MeshComp, Animation);

	GetWorld()->SpawnActor<AActor>(SpawningActor, MeshComp->GetSocketTransform(InSocketName));
}
