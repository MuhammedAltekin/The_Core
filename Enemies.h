// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Enemies.generated.h"
/**
 * 
 */

UENUM(BlueprintType)
enum EEnemies
{
	Bomber			UMETA(DisplayName = "Bomber"),
	Long_Ranged		UMETA(DisplayName = "Long Ranged"),
	Boss			UMETA(DisplayName = "Boss"),
	Melee			UMETA(DisplayName = "Melee"),
	Ranged			UMETA(DisplayName = "Ranged"),
	Tank			UMETA(DisplayName = "Tank"),
	Buffer			UMETA(DisplayName = "Buffer"),
	All_Enemy		UMETA(DisplayName = "All_Enemy")
};
