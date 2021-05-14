// Fill out your copyright notice in the Description page of Project Settings.

#include "UBTorpedo.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Controller.h"
#include "UBTransport.h"
#include "UBDestroyer.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUBTorpedo::AUBTorpedo()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Is responsible for the pawn movement
	CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");

	//Set the mesh for the torpedo
	Torpedo = CreateDefaultSubobject <USkeletalMeshComponent>("Torpedo");

	//Creates the collision sphere

	_collision = CreateDefaultSubobject<USphereComponent>("Collision");

	_collision->SetSphereRadius(80.0f);

	_collision->SetHiddenInGame(true);

	//Detects the collision. In case of an overlap, it calls the function ATorpedo::OnHit
	_collision->OnComponentBeginOverlap.AddDynamic(this, &AUBTorpedo::OnHit);

	//Sets the attachments
	Torpedo->SetupAttachment(_collision);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AUBTorpedo::OnHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AUBTransport* MyCharacter = Cast<AUBTransport>(OtherActor);

	AUBDestroyer* MyDestroyer = Cast<AUBDestroyer>(OtherActor);

	if (MyCharacter || MyDestroyer)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());

		MulticastRPCFunction_PlayExplosionEffect();

		Destroy();
	}
}

// Called when the game starts or when spawned
void AUBTorpedo::BeginPlay()
{
	Super::BeginPlay();
	
	InitialLifeSpan = 20;
}

// Called every frame
void AUBTorpedo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(0.5);
}

// Called to bind functionality to input
void AUBTorpedo::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUBTorpedo::MulticastRPCFunction_PlayExplosionEffect_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}

bool AUBTorpedo::MulticastRPCFunction_PlayExplosionEffect_Validate()
{
	return true;
}

void AUBTorpedo::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

