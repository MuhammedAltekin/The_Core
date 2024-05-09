// Fill out your copyright notice in the Description page of Project Settings.


#include "Solider.h"
#include "EnemyBase.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerGameState.h"
#include "Particles/ParticleSystemComponent.h"

#define Print(Key,Time,Color,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,TEXT(Text))
#define PrintF(Key,Time,Color,Type,Text) if(GEngine) GEngine->AddOnScreenDebugMessage(Key,Time,FColor::Color,FString::Printf(TEXT(Type),Text))
#define String(Text) FString(TEXT(Text))
#define OnceTimer(TimerHandle,Promph,Loop,Time,Delay) GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=,this](){ Promph }), Time, Loop,Delay)

// Sets default values
ASolider::ASolider()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

}


// Called when the game starts or when spawned
void ASolider::BeginPlay()
{
	Super::BeginPlay();

	//Cast Controller
	PlayerController = Cast<APlayerController>(Controller);
	PlayerController->bShowMouseCursor = true;
	PlayerController->DefaultMouseCursor = EMouseCursor::Default;

	//Cast Game State
	PlayerGameState = Cast<APlayerGameState>(UGameplayStatics::GetGameState(GetWorld()));

	//----------------Setting Input-----------------//
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	
	OnceTimer(CdTh,
		CooldownCounting();
		,true, 1, 0);
	PlayerGameState->WholeTimers.Add(CdTh);
	PlayerGameState->Player = this;
}

// Called to bind functionality to input
void ASolider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASolider::Move);

		// RMB
		EnhancedInputComponent->BindAction(RMB_Action, ETriggerEvent::Started, this, &ASolider::ScopeOpen);
		EnhancedInputComponent->BindAction(RMB_Action, ETriggerEvent::Completed, this, &ASolider::ScopeClose);

		// LMB
		EnhancedInputComponent->BindAction(LMB_Action, ETriggerEvent::Triggered, this, &ASolider::LMB_Triggered);

		//LShift
		EnhancedInputComponent->BindAction(LShift_Action, ETriggerEvent::Started, this, &ASolider::Dash_Started);

		//Key F
		EnhancedInputComponent->BindAction(KeyF_Action, ETriggerEvent::Started, this, &ASolider::SlowMotion_Started);

		// Look
		EnhancedInputComponent->BindAction(Look_Action, ETriggerEvent::Triggered, this, &ASolider::Look);
	}
	else
	{
	}

}


float ASolider::GetBulletDamage_Implementation()
{
	return BulletDamage;
}

void ASolider::IncreaseHealthArmor_Implementation(float HealthValue, float ArmorValue)
{
	IPlayerCharacterInterface::IncreaseHealthArmor_Implementation(HealthValue, ArmorValue);

	Health += HealthValue;
	MaxHealth += HealthValue;
	Armor += ArmorValue;
}


// Called every frame
void ASolider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Scope && !isDead)
	{
		VelocityDirection.Yaw = CalcCachedRot();

		//Apply RotYaw
		SetActorRotation(FRotator(0, CalcEaseRotation(), 0));

		//Turn Cache
		CurrentYaw = GetActorRotation().Yaw;
		if (!bTurnCache)
		{
			bTurnCache = true;

			OnceTimer(TurnCacheTh,
				CachedYaw = GetActorRotation().Yaw;
				bTurnCache = false;
				GetWorld()->GetTimerManager().ClearTimer(TurnCacheTh);
			, false, 1,.5f);

		}
		
		

		//Apply Turn Caches
		TurnRate = CalcTurnRate();

		//Calc Rotating
		if (GetSpeed() > 10)
		{
			Rotating = false;
		}
		else
		{
			if (TurnRate == 0)
			{
				Rotating = false;
			}
			else
			{
				Rotating = true;
			}
		}

	}
	TickTimerEvent();
}

float ASolider::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	Health -= (DamageAmount - ((DamageAmount / 100) * Armor));
	if (Health <= 0 && !isDead)
	{
		DeathEvent();
		isDead = true;
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ASolider::Move(const FInputActionValue& Value)
{
	if (!SkillUsing)
	{
		// input is a Vector2D
		const FVector2D MovementVector = Value.Get<FVector2D>();

		// find out which way is forward
		const FRotator Rotation = this->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASolider::Look(const FInputActionValue& Value)
{
	
	if (!TPSMod)
	{
		FVector2D MovementVector = Value.Get<FVector2D>() / 2;
		MovementVector = UKismetMathLibrary::MapRangeClamped(CameraComponent->FieldOfView, 0, 110, 0, 1) * MovementVector * PlayerGameState->MouseSensitivity;
		AddControllerYawInput(MovementVector.X);
		AddControllerPitchInput(MovementVector.Y);
	}
}

void ASolider::Dash_Started()
{
	if (CD_Dash <= 0)
	{
		switch (PlayerGameState->DashLevel)
		{
		case 0:
			break;
		case 1:
			DashEvent(1.5f, 2);
			break;
		case 2:
			DashEvent(1.5f, 1);
			break;
		case 3:
			DashEvent(2, 1);
			break;
		case 4:
			DashEvent(2, 0);
			break;
		default:;
		}
	}
}

void ASolider::DashEvent(float DashValue, float Cooldown)
{
	CD_Dash = Cooldown;
	GetMesh()->SetVisibility(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	UParticleSystemComponent* Emitter = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FXDashStart, GetActorLocation(), FRotator(0), FVector(5));
	Emitter->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(GetWorld());
	UGameplayStatics::SpawnSoundAttached(Sound_TeleportStart,RootComponent);
	SetActorLocation(FVector(CalcDashLocation(DashValue)), true, nullptr, ETeleportType::None);
	//Delay 1
	FTimerHandle OnceTh;
	OnceTimer(OnceTh,
		UParticleSystemComponent * Emitter2 = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FXDashEnd, GetActorLocation(), FRotator(0), FVector(5));
		Emitter2->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(GetWorld());

		//Delay 2 Start
		FTimerHandle OnceTh2;
		OnceTimer(OnceTh2,
			UGameplayStatics::SpawnSoundAttached(Sound_TeleportEnd, RootComponent);
			GetMesh()->SetVisibility(true);
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		, false, 1, UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * .1f);
		//Delay 2 End
	, false, 1, UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * .2f);
	//Delay 1 End
}

FVector ASolider::FindTargetLoc()
{
	FHitResult Hr;
	if (TPSMod)
	{
		
		PlayerController->GetHitResultUnderCursorByChannel(TraceTypeQuery3, true,Hr);
		TargetLocation = Hr.ImpactPoint;
		if (Cast<AEnemyBase>(Hr.GetActor()))
		{
			AimOnTarget = true;
		}
		else
		{
			AimOnTarget = false;
		}
	}
	else
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);
		const FVector TraceStart = CameraComponent->GetComponentLocation() + 350 * CameraComponent->GetForwardVector();
		const FVector TraceEnd = CameraComponent->GetComponentLocation() + 100000000 * CameraComponent->GetForwardVector();
		UKismetSystemLibrary::LineTraceSingle(GetWorld(),TraceStart, TraceEnd, TraceTypeQuery1, false, IgnoredActors, EDrawDebugTrace::None, Hr ,true);
		TargetLocation = Hr.ImpactPoint;
		if (Cast<AEnemyBase>(Hr.GetActor()))
		{
			AimOnTarget = true;

		}
		else
		{
			AimOnTarget = false;
		}
	}
	if (AimOnTarget)
	{
		//const float Distance = UKismetMathLibrary::Vector_Distance(Hr.GetActor()->GetActorLocation(), GetActorLocation());
		//TargetLocation = UKismetMathLibrary::MapRangeClamped(Distance,0,12000,0,2) * Hr.GetActor()->GetVelocity() + Hr.GetActor()->GetActorLocation() + FVector(0,0,50);
	}
	return TargetLocation;
}

float ASolider::EaseRotation(const FRotator& EaseB) const
{
	const bool SelectFloatBool = UKismetMathLibrary::GreaterEqual_DoubleDouble(GetSpeed(), 1);
	const float EaseAlpha = UKismetMathLibrary::SelectFloat(1, 0.2f, SelectFloatBool);
	const FRotator EaseReturn = UKismetMathLibrary::REase(GetActorRotation(), EaseB,EaseAlpha,true,EEasingFunc::Linear,2,2);
	return EaseReturn.Yaw;
}

float ASolider::GetSpeed() const
{
	return GetVelocity().Length();
}

float ASolider::CalcTurnRate() const
{
	return UKismetMathLibrary::Abs(CurrentYaw - CachedYaw);
}

double ASolider::CalcEaseRotation()
{
	const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FindTargetLoc());
	return EaseRotation(LookAtRot);
}

double ASolider::CalcCachedRot()
{
	const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FindTargetLoc());
	return LookAtRot.Yaw - 2;
}

void ASolider::CooldownCounting()
{
	CD_SlowMotion--;
	CD_Dash--;
	CD_Bomb--;
	CD_Ultimate--;
}

FVector ASolider::CalcDashLocation(float DashMultiplier) const
{
	const FRotator SelectDashType = UKismetMathLibrary::SelectRotator(UKismetMathLibrary::MakeRotFromX(GetVelocity()), GetMesh()->GetSocketRotation(FName("Forward")), Scope);
	const FVector Value = DashDistance * DashMultiplier * UKismetMathLibrary::GetForwardVector(SelectDashType);
	return GetActorLocation() + Value;
}

void ASolider::ScopeOpen()
{
	Scope = true;
	CanShoot = true;
	GetCharacterMovement()->MaxWalkSpeed = 400;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	DesiredFov = UKismetMathLibrary::SelectFloat(80, 60, TPSMod);
}

void ASolider::ScopeClose()
{
	Scope = false;
	GetCharacterMovement()->MaxWalkSpeed = 600;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CanShoot = false;
	DesiredFov = UKismetMathLibrary::SelectFloat(70,110,TPSMod);
}

void ASolider::TickTimerEvent()
{
	//Setting Fov
	if (!(UKismetMathLibrary::NearlyEqual_FloatFloat(GetCameraComponent()->FieldOfView,DesiredFov,.1f)))
	{
		const float Fov = UKismetMathLibrary::Ease(GetCameraComponent()->FieldOfView, DesiredFov, .3f, EEasingFunc::CircularInOut);
		GetCameraComponent()->SetFieldOfView(Fov);
	}

	//Slow Motion
	SlowMotionEvent();
	
}

void ASolider::LMB_Triggered()
{
	if (Scope && !SkillUsing)
	{
		if (!ShootingTriggered)
		{
			ShootingTriggered = true;
			ShootingEvent(TargetLocation,GunType);
			const float Density = UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * PlayerShootDensityMultiply * GunShootDensity;
			OnceTimer(GunShootingTh,
				ShootingTriggered = false;
				GetWorld()->GetTimerManager().ClearTimer(GunShootingTh);
			, false, 1, Density);
		}	
	}
}


void ASolider::ShootingEvent(FVector TargetLoc, EGunType Gun)
{
	if (CanShoot)
	{
		const float PitchMultiply = UGameplayStatics::GetGlobalTimeDilation(GetWorld()) * 3;

		//Assault
		if (Gun == EGunType::Assault)
		{
			UGameplayStatics::SpawnSoundAttached(SFXGunShoot, GetMesh(), FName(""), FVector(0), FRotator(0), EAttachLocation::KeepRelativeOffset, true, 1, PitchMultiply, 0, nullptr, nullptr, true);
			const FRotator BulletLookAtRot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(FName("Gun_Barrel")), TargetLoc);
			GetWorld()->SpawnActor<AActor>(PlayerGunBullet, GetMesh()->GetSocketLocation(FName("Gun_Barrel")), BulletLookAtRot);
			UGameplayStatics::PlayWorldCameraShake(GetWorld(), AssaultShake, GetActorLocation(), 1500, 0, 1);
			
		}

		//Laser
		if (Gun == EGunType::Laser)
		{
			LaserShootBP(TargetLoc);
		}

		//Shotgun
		if (Gun == EGunType::Shotgun)
		{
			PlayAnimMontage(ShotgunMontage, 1);
			const FRotator BulletLookAtRot = UKismetMathLibrary::FindLookAtRotation(GetMesh()->GetSocketLocation(FName("Gun_Barrel")), TargetLoc);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ShotgunShootVFX,GetMesh()->GetSocketLocation(FName("Gun_Barrel")),BulletLookAtRot)->CustomTimeDilation = 2;
			for (int i = 0; i < ShotgunBulletCount; i++)
			{
				const FVector ConeDirection = UKismetMathLibrary::GetForwardVector(BulletLookAtRot);
				const FRotator RandomRot = UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ConeDirection, PlayerGameState->GetShotgunRadius()));
				GetWorld()->SpawnActor<AActor>(ShotgunBullet, GetMesh()->GetSocketLocation(FName("Gun_Barrel")),RandomRot);
			}
		}
	}
}

void ASolider::KnockbackBegin()
{
	Knockback = true;
	Scope = false;
	Rotating = false;
}

void ASolider::KnockbackEnd()
{
	Knockback = false;
}

void ASolider::SlowMotion_Started()
{
	if (!PlayerGameState->LevelingProcess && CD_SlowMotion <= 0 && !SlowMotion && PlayerGameState->SlowMotionLevel > 0)
	{
		
		SlowMotion = true;
		bSlowMotionBegin = true;
		DesiredTimeDilation = .01f;
		PlaySlowMoTalk();
		switch (PlayerGameState->SlowMotionLevel)
			{
			case 1:
				CD_SlowMotion = 20;
				SlowMo_Multiplier = .5f;
				break;
			case 2:
				CD_SlowMotion = 15;
				SlowMo_Multiplier = 1;
				break;
			case 3:
				CD_SlowMotion = 15;
				SlowMo_Multiplier = 1.5f;
				break;
			case 4:
				CD_SlowMotion = 10;
				SlowMo_Multiplier = 2;
				break;
			default:;
			}

	}
}

void ASolider::SlowMotionEvent()
{
	if (SlowMotion)
	{
		if (bSlowMotionBegin)
		{
			if (!UKismetMathLibrary::NearlyEqual_FloatFloat(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), DesiredTimeDilation, .00001f))
			{
				const float TimeDilation = UKismetMathLibrary::Ease(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), DesiredTimeDilation, .2f, EEasingFunc::CircularInOut);
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
				CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(GetWorld());
				
			}
			else
			{
				SlowMotion = false;
				FTimerHandle OnceTh;
				OnceTimer(OnceTh,
					bSlowMotionBegin = false;
					SlowMotion = true;
				, false, 1, .01f * SlowMo_Multiplier);
			}

		}
		else
		{
			DesiredTimeDilation = 1;
			if (!UKismetMathLibrary::NearlyEqual_FloatFloat(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), DesiredTimeDilation, .00001f))
			{
				const float TimeDilation = UKismetMathLibrary::Ease(UGameplayStatics::GetGlobalTimeDilation(GetWorld()), DesiredTimeDilation, .15f, EEasingFunc::CircularInOut);
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
				CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(GetWorld());
				
			}
			else
			{
				SlowMotion = false;
				
			}
		}

	}
}




