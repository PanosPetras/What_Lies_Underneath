// Fill out your copyright notice in the Description page of Project Settings.

#include "UBDestroyerProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "UBU995.h"
#include "UBDestroyer.h"
#include "UBTransport.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Controller.h"
#include "WLU_MPGameModeBase.h"

// Sets default values
AUBDestroyerProjectile::AUBDestroyerProjectile()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Is responsible for the pawn movement
	CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement");

	//Set the mesh for the torpedo
	Torpedo = CreateDefaultSubobject <USkeletalMeshComponent>("Torpedo");

	_collision = CreateDefaultSubobject<USphereComponent>("Collision");

	_collision->SetSphereRadius(80.0f);

	_collision->SetHiddenInGame(true);

	//Detects the collision. In case of an overlap, it calls the function ATorpedo::OnHit
	_collision->OnComponentBeginOverlap.AddDynamic(this, &AUBDestroyerProjectile::OnHit);

	//Sets the attachments
	Torpedo->SetupAttachment(_collision);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AUBDestroyerProjectile::OnHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AUBU995* MySub = Cast<AUBU995>(OtherActor);
	if (MySub)
	{
		MulticastRPCFunction_PlayEffects();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		
		MySub->TypicalSpeed = 7;

		MySub->OnHit();
		
		Destroy();
	}
}

// Called when the game starts or when spawned
void AUBDestroyerProjectile::BeginPlay()
{
	Super::BeginPlay();

	InitialLifeSpan = 20;
	
}

// Called every frame
void AUBDestroyerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward(1.1);
}

// Called to bind functionality to input
void AUBDestroyerProjectile::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUBDestroyerProjectile::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void AUBDestroyerProjectile::MulticastRPCFunction_PlayEffects_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
}