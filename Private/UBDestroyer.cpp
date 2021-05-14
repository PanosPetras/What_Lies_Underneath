// Fill out your copyright notice in the Description page of Project Settings.

#include "UBDestroyer.h"
#include "UBU995.h"
#include "Kismet/GameplayStatics.h"
#include "UBTorpedo.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "UBLootZone.h"
#include "UBDestroyerProjectile.h"
#include "DrawDebugHelpers.h"

// Sets default values
AUBDestroyer::AUBDestroyer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sunk = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(SceneRoot);

	SceneComp1 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp1"));
	SceneComp1->SetupAttachment(SceneRoot);

	GunComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunComp"));
	GunComp1->SetupAttachment(SceneComp1);

	SceneComp2 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp2"));
	SceneComp2->SetupAttachment(GunComp1);

	GunBarellComp1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunBarellComp"));
	GunBarellComp1->SetupAttachment(SceneComp2);

	SceneComp3 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp3"));
	SceneComp3->SetupAttachment(SceneRoot);

	GunComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunComp2"));
	GunComp2->SetupAttachment(SceneComp3);

	SceneComp4 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp4"));
	SceneComp4->SetupAttachment(GunComp2);

	GunBarellComp2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunBarellComp2"));
	GunBarellComp2->SetupAttachment(SceneComp4);

	Flag = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FlagComp"));
	Flag->SetupAttachment(MeshComp);

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponents"));

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &AUBDestroyer::OnHit);

	PawnSensingComp->OnHearNoise.AddDynamic(this, &AUBDestroyer::OnNoiseHeard);
}

// Called when the game starts or when spawned
void AUBDestroyer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUBDestroyer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (sunk == true)
	{
		FRotator Rot = GetActorRotation();

		MoveDown(-1);

		if (Rot.Pitch < 7)
		{
			Rot.Pitch += 0.003;
			SetActorRotation(Rot);
		}

	}

	FVector Loc = GetActorLocation();

	if (Loc.Z < 0)
	{
		Destroy();
	}
}

// Called to bind functionality to input
void AUBDestroyer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUBDestroyer::OnHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AUBTorpedo* Torp = Cast<AUBTorpedo>(OtherActor);

	if (Torp && sunk == false)
	{
		sunk = true;

		UWorld* World = GetWorld();

		FRotator SpawnRotation = MeshComp->GetComponentRotation();

		FVector SpawnLocation = MeshComp->GetComponentLocation();

		SpawnLocation.Z += 200;

		World->SpawnActor<AUBLootZone>(ProjectileClass, SpawnLocation, SpawnRotation);

		Overlap();

	}
}

void AUBDestroyer::OnNoiseHeard(APawn * Noiseinstigator, const FVector & Location, float Volume)
{
	if (Noiseinstigator == nullptr)
	{
		return;
	}
	
	if (Location.Z > 200)
	{
		if (sunk == false)
		{
			FVector Direction = Location - GetActorLocation();
			Direction.Normalize();

			FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
			NewLookAt.Roll = 90.0f;
			NewLookAt.Pitch = 0.0f;

			SceneComp1->SetWorldRotation(NewLookAt);
			SceneComp3->SetWorldRotation(NewLookAt);

			AUBU995* Sub = Cast<AUBU995>(Noiseinstigator);

			if (Sub && Sub->TypicalSpeed != 7)
			{
				if (SubSpotted == false)
				{
					MulticastRPCFunction_SirensSoundEffect();

					SubSpotted = true;

					GetWorldTimerManager().ClearTimer(UnusedHandle);

					GetWorldTimerManager().SetTimer(UnusedHandle, this, &AUBDestroyer::OnUnusedTimerEnd, 4.0f);

				}
			}
		}
	}
}

void AUBDestroyer::OnUnusedTimerEnd()
{
	MulticastRPCFunction_FireEffect();

	UWorld* World = GetWorld();

	const FRotator SpawnRotation1 = SceneComp2->GetComponentRotation();

	const FVector SpawnLocation1 = SceneComp2->GetComponentLocation();

	const FRotator SpawnRotation2 = SceneComp4->GetComponentRotation();

	const FVector SpawnLocation2 = SceneComp4->GetComponentLocation();

	World->SpawnActor<AUBDestroyerProjectile>(Projectile, SpawnLocation1, SpawnRotation1);

	World->SpawnActor<AUBDestroyerProjectile>(Projectile, SpawnLocation2, SpawnRotation2);

	GetWorldTimerManager().ClearTimer(SpottedHandle);

	GetWorldTimerManager().SetTimer(SpottedHandle, this, &AUBDestroyer::OnSpottedTimerEnd, 9.0f);
}

void AUBDestroyer::OnSpottedTimerEnd()
{
	SubSpotted = false;
}

void AUBDestroyer::MulticastRPCFunction_FireEffect_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, SceneComp2->GetComponentLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireEffect, SceneComp4->GetComponentLocation());
}

bool AUBDestroyer::MulticastRPCFunction_FireEffect_Validate()
{
	return true;
}

bool AUBDestroyer::MulticastRPCFunction_SirensSoundEffect_Validate()
{
	return true;
}

void AUBDestroyer::MulticastRPCFunction_SirensSoundEffect_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, Sirens, GetActorLocation());
}

void AUBDestroyer::MoveDown_Implementation(float Value)
{
	AddMovementInput(GetActorUpVector(), Value);
}

bool AUBDestroyer::MoveDown_Validate(float Value)
{
	return true;
}
