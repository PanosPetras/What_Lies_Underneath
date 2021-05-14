// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/UBLootZone.h"
#include "UBU995.h"
#include "Components/BoxComponent.h"

// Sets default values
AUBLootZone::AUBLootZone()
{

	InitialLifeSpan = 30.0f;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AUBLootZone::OnOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AUBLootZone::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AUBLootZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AUBLootZone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUBLootZone::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AUBU995* Sub = Cast<AUBU995>(OtherActor);
	if (Sub)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

		Sub->bCanLoot = true;

		Sub->LootZone = this;

		Sub->LZDisplay(true);
	}
}

void AUBLootZone::OnOverlapEnd(UPrimitiveComponent * Comp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AUBU995* Sub = Cast<AUBU995>(OtherActor);
	if (Sub)
	{
		Sub->bCanLoot = false;

		Sub->LZDisplay(false);
	}
}
