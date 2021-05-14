#include "UBU995.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/TextRenderComponent.h"
#include "Engine/Engine.h"
#include "UBTorpedo.h"
#include "Components/SphereComponent.h"

// Sets default values
AUBU995::AUBU995()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Submarine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	// Create a spring arm component

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetOffset = FVector(0.f, 0.f, 100.f);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 550.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 7.f;
	SpringArm->bInheritPitch = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritRoll = false;

	//Sets the camera

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->FieldOfView = 90.f;
	FVector CamLoc = Camera->GetComponentLocation();
	CamLoc.Z += 00;
	Camera->SetRelativeLocation(CamLoc);
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	Fire1 = CreateDefaultSubobject<USphereComponent>(TEXT("Fire1"));
	Fire1->SetupAttachment(RootComponent);

	Fire2 = CreateDefaultSubobject<USphereComponent>(TEXT("Fire2"));
	Fire2->SetupAttachment(RootComponent);

	FlagOne = CreateDefaultSubobject<USkeletalMeshComponent>("FlagOne");
	FlagTwo = CreateDefaultSubobject<USkeletalMeshComponent>("FlagTwo");
	FlagOne->SetupAttachment(RootComponent);
	FlagTwo->SetupAttachment(RootComponent);

	Torp = CreateDefaultSubobject<USphereComponent>("TorpedoLauncher");
	Torp->SetupAttachment(RootComponent);

	fuel = 30000;
	TurnRate = 0;
}

void AUBU995::UpdateCameraRotPitch(float Value)
{
	CameraRotPitch = Value;
}

void AUBU995::UpdateCameraRotYaw(float Value)
{
	CameraRotYaw = Value;
}

void AUBU995::OnFire()
{
	if (TypicalSpeed != 7 && TorpedoCooldown == 0 && AvTorps > 0)
	{
		ServerFire();
		UGameplayStatics::PlaySoundAtLocation(this, LaunchingSound, GetActorLocation());
	
		TorpedoCooldown = 1000;

		AvTorps += -1;
	}
}

// Called when the game starts or when spawned
void AUBU995::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUBU995::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TorpedoCooldown > 0)
	{
		TorpedoCooldown = TorpedoCooldown - 1;
	}

	MakeNoise(1.0f, Instigator);

}

// Called to bind functionality to input
void AUBU995::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AUBU995::MoveForward);

	PlayerInputComponent->BindAxis("Submerge", this, &AUBU995::Submerge);

	PlayerInputComponent->BindAxis("Turn", this, &AUBU995::UpdateCameraRotYaw);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AUBU995::OnFire);

	PlayerInputComponent->BindAxis("MoveRight", this, &AUBU995::OnMoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &AUBU995::UpdateCameraRotPitch);

	PlayerInputComponent->BindAxis("Resupply", this, &AUBU995::Resupply);

}

void AUBU995::MoveForward(float Value)
{
	FVector NewLocation = GetActorLocation();

	FRotator Rot = GetActorRotation();

	if (Value != 0)
	{
		if (TypicalSpeed != 7 && Value <= 6)
		{

			TypicalSpeed = Value;
		}
	}

	if (fuel > 0)
	{
		if (TypicalSpeed == 1)
		{
			if (ActualSpeed > -0.2)
			{
				ActualSpeed = ActualSpeed - 0.0003;
			}
			if (ActualSpeed < -0.2)
			{
				ActualSpeed = ActualSpeed + 0.0003;
			}

			AddMovementInput(GetActorForwardVector(), ActualSpeed);
		}

		if (TypicalSpeed == 2)
		{
			if (ActualSpeed < 0)
			{
				ActualSpeed = ActualSpeed + 0.0003;
			}
			if (ActualSpeed > 0)
			{
				ActualSpeed = ActualSpeed - 0.0003;
			}

			AddMovementInput(GetActorForwardVector(), ActualSpeed);

		}

		if (TypicalSpeed == 3)
		{
			if (ActualSpeed < 0.1)
			{
				ActualSpeed = ActualSpeed + 0.0003;
			}
			if (ActualSpeed > 0.1)
			{
				ActualSpeed = ActualSpeed - 0.0003;
			}

			AddMovementInput(GetActorForwardVector(), ActualSpeed);
		}

		if (TypicalSpeed == 4)
		{
			if (ActualSpeed < 0.2)
			{
				ActualSpeed = ActualSpeed + 0.0003;
			}
			if (ActualSpeed > 0.2)
			{
				ActualSpeed = ActualSpeed - 0.0003;
			}

			AddMovementInput(GetActorForwardVector(), ActualSpeed);
		}

		if (TypicalSpeed == 5)
		{
			if (ActualSpeed < 0.3)
			{
				ActualSpeed = ActualSpeed + 0.0003;
			}
			if (ActualSpeed > 0.3)
			{
				ActualSpeed = ActualSpeed - 0.0003;
			}

			AddMovementInput(GetActorForwardVector(), ActualSpeed);
		}

		if (TypicalSpeed == 6)
		{
			if (ActualSpeed < 0.4)
			{
				ActualSpeed = ActualSpeed + 0.0003;
			}
			if (ActualSpeed > 0.4)
			{
				ActualSpeed = ActualSpeed - 0.0003;
			}

			AddMovementInput(GetActorForwardVector(), ActualSpeed);
		}

		if (TypicalSpeed == 7)
		{
			if (ActualSpeed < 0)
			{
				ActualSpeed = ActualSpeed + 0.0008;
			}
			if (ActualSpeed > 0)
			{
				ActualSpeed = ActualSpeed - 0.0008;
			}

			AddMovementInput(GetActorForwardVector(), ActualSpeed);

		}

		if (ActualSpeed > 0)
		{
			fuel += (ActualSpeed * -1);
		}
		else
		{
			fuel += ActualSpeed;
		}
	}
	else
	{
		if (ActualSpeed < 0)
		{
			ActualSpeed = ActualSpeed + 0.0003;
		}
		if (ActualSpeed > 0)
		{
			ActualSpeed = ActualSpeed - 0.0003;
		}
		AddMovementInput(GetActorForwardVector(), ActualSpeed);
	}

}

void AUBU995::Submerge(float Value)
{
	FVector NewLocation = GetActorLocation();

	FRotator Rot = GetActorRotation();

	if (TypicalSpeed != 7)
	{
		if (Value > 0 && NewLocation.Z < 220)
		{
			AddMovementInput(GetActorUpVector(), Value);
		}
		if (Value < 0)
		{
			AddMovementInput(GetActorUpVector(), Value);
		}


		if (NewLocation.Z > 110)
		{
			FlagOne->SetHiddenInGame(false);
			FlagTwo->SetHiddenInGame(false);
		}
		else if (NewLocation.Z < 110)
		{
			FlagOne->SetHiddenInGame(true);
			FlagTwo->SetHiddenInGame(true);
		}

		if (NewLocation.Z > 120 && Rot.Pitch > 0)
		{
			Rot.Pitch += -0.02;
		}
	}
}

void AUBU995::Turn(float Value)
{
	FRotator Rot = SpringArm->GetComponentRotation();

	Rot.Yaw += Value;

	SpringArm->SetWorldRotation(Rot);
}

void AUBU995::OnMoveRight(float Value)
{
	if (TypicalSpeed == 7)
	{
		return;
	}
	FRotator Rot = GetActorRotation();

	float Acceleration = 0.0001 * (ActualSpeed * 5);

	if (Value > 0 && TurnRate < 0.11)
	{
		TurnRate = TurnRate + Acceleration;
	}
	if (Value < 0 && TurnRate > -0.11)
	{
		TurnRate = TurnRate - Acceleration;
	}
	if (Value == 0)
	{
		if (TurnRate > 0)
		{
			TurnRate += -0.0004;
		}
		if (TurnRate < 0)
		{
			TurnRate += 0.0004;
		}
	}
	AddControllerYawInput(TurnRate);
}

void AUBU995::Resupply(float Value)
{
	if (Value != 0 && bCanResupply == true)
	{
		AvTorps = 14;

		fuel = 30000;
	}
	if (Value != 0 && bCanLoot == true)
	{
		AvTorps += 2;

		fuel += 2000;

		LootZone->Destroy();

		bCanLoot = false;
	}
}

void AUBU995::OnHandleEnd1()
{
	MulticastRPCFunction_PlayEffects1();

	GetWorldTimerManager().ClearTimer(DestroyedHandle);

	GetWorldTimerManager().SetTimer(DestroyedHandle, this, &AUBU995::MulticastRPCFunction_PlayEffects2, 0.7f);

	GetWorldTimerManager().ClearTimer(DestroyedHandle2);

	GetWorldTimerManager().SetTimer(DestroyedHandle2, this, &AUBU995::MulticastRPCFunction_PlayEffects2, 1.4f);
}

void AUBU995::MulticastRPCFunction_PlayEffects1_Implementation()
{
	FVector Loc = GetActorLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, Loc);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Loc);

	FVector Loctor = Fire1->GetComponentLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Loctor);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, Loctor);

	FVector LoctorV2 = Fire2->GetComponentLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, LoctorV2);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, LoctorV2);

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

	FlagOne->SetHiddenInGame(true);

	FlagTwo->SetHiddenInGame(true);
}

void AUBU995::MulticastRPCFunction_PlayEffects2_Implementation()
{
	FVector Loc = GetActorLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Loc);

	FVector Loctor = Fire1->GetComponentLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Loctor);

	FVector LoctorV2 = Fire2->GetComponentLocation();

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, LoctorV2);

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}

void AUBU995::OnHit()
{
	GetWorldTimerManager().ClearTimer(DestroyedHandle);

	GetWorldTimerManager().SetTimer(DestroyedHandle, this, &AUBU995::OnHandleEnd1, 5.0f);

	GameOverScreen();

	TypicalSpeed = 7;
}

void AUBU995::ServerFire_Implementation()
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{

			const FRotator SpawnRotation = Torp->GetComponentRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = Torp->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = this;

			// spawn the projectile at the muzzle
			World->SpawnActor<AUBTorpedo>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
}

bool AUBU995::ServerFire_Validate()
{
	return true;
}
