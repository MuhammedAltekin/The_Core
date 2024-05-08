// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class ALEYIKAPAKPROJESI_API SDamageNumbers : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDamageNumbers)
	{}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	UPROPERTY(BlueprintReadWrite)
	int Damage = 0;
};
