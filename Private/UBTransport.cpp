// Fill out your copyright notice in the Description page of Project Settings.

#include "UBTransport.h"
#include "UBLootZone.h"
#include "UBTorpedo.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AUBTransport::AUBTransport()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Is responsible for the pawn movement
	CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");

	//Sets the ship mesh
	Ship = CreateDefaultSubobject<USkeletalMeshComponent>("Ship");

	//Create the first container
	Container1 = CreateDefaultSubobject<USkeletalMeshComponent>("ContainerNo1");
	Container1->SetupAttachment(Ship);

	//Create the second container
	Container2 = CreateDefaultSubobject<USkeletalMeshComponent>("ContainerNo2");
	Container2->SetupAttachment(Ship);

	//Create the second container 
	Container3 = CreateDefaultSubobject<USkeletalMeshComponent>("ContainerNo3");
	Container3->SetupAttachment(Ship);

	//Create the fourth container
	Container4 = CreateDefaultSubobject<USkeletalMeshComponent>("ContainerNo4");
	Container4->SetupAttachment(Ship);
	
	//Create the two flags
	FlagOne = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FlagOne"));
	FlagOne->SetupAttachment(Ship);

	FlagTwo = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FlagTwo"));
	FlagTwo->SetupAttachment(Ship);

	SetReplicates(true);
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AUBTransport::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUBTransport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (sunk == true)
	{
		FRotator Rot = GetActorRotation();

		MoveDown(-0.02);

		if (Rot.Pitch < 7)
		{
			Rot.Pitch += 0.01;
			SetActorRotation(Rot);
		}

	}

	FVector Loc = GetActorLocation();

	if (Loc.Z < 0)
	{
		Destroy();
	}

}

void AUBTransport::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AUBTorpedo* Torp = Cast<AUBTorpedo>(OtherActor);

	if (Torp && sunk == false)
	{
		sunk = true;

		Overlap();

		SpawnLootZone();
	}
}

void AUBTransport::MoveDown(float Value)
{
	AddMovementInput(GetActorUpVector(), Value);
}

bool AUBTransport::SpawnLootZone_Validate()
{
	return true;
}

void AUBTransport::SpawnLootZone_Implementation()
{
	UWorld* World = GetWorld();

	FRotator SpawnRotation = Ship->GetComponentRotation();

	FVector SpawnLocation = Ship->GetComponentLocation();

	SpawnLocation.Z += -400;

	World->SpawnActor<AUBLootZone>(ProjectileClass, SpawnLocation, SpawnRotation);
}

// Called to bind functionality to input
void AUBTransport::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

