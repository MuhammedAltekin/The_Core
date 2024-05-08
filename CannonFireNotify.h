// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CannonFireNotify.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API UCannonFireNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditInstanceOnly)
	TSubclassOf<AActor> SpawningActor;
	UPROPERTY(EditInstanceOnly)
	FName InSocketName;
};
