// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ShockBlastNotify.generated.h"

/**
 * 
 */
UCLASS()
class ALEYIKAPAKPROJESI_API UShockBlastNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditInstanceOnly)
	float Radius;
	UPROPERTY(EditInstanceOnly)
	float PushForce;
	UPROPERTY(EditInstanceOnly)
	float BaseDamage;

};
