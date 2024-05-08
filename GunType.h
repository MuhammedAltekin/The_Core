// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GunType.generated.h"
/**
 *
 */

UENUM(BlueprintType)
enum EGunType
{
	Assault			UMETA(DisplayName = "Assault"),
	Laser			UMETA(DisplayName = "Laser"),
	Shotgun			UMETA(DisplayName = "Shotgun")
};
