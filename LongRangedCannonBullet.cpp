// Fill out your copyright notice in the Description page of Project Settings.


#include "LongRangedCannonBullet.h"

#include "PlayerGameState.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALongRangedCannonBullet::ALongRangedCannonBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALongRangedCannonBullet::BeginPlay()
{
	Super::BeginPlay();
	APlayerGameState* PlayerGameState = Cast<APlayerGameState>(GetWorld()->GetGameState());
	const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerGameState->GetTarget()->GetActorLocation()));
	const float Distance = UKismetMathLibrary::Vector_Distance(PlayerGameState->GetTarget()->GetActorLocation(), GetActorLocation());
	const FVector VectorValue = ForwardVector * Distance;
	Velocity = FVector(VectorValue.X,VectorValue.Y,1400);;
}

// Called every frame
void ALongRangedCannonBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}