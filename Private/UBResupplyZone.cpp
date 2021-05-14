// Fill out your copyright notice in the Description page of Project Settings.

#include "UBResupplyZone.h"
#include "UBU995.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AUBResupplyZone::AUBResupplyZone()
{

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AUBResupplyZone::OnOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AUBResupplyZone::OnOverlapEnd);
}

// Called when the game starts or when spawned
void AUBResupplyZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AUBResupplyZone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUBResupplyZone::OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AUBU995* Sub = Cast<AUBU995>(OtherActor);
	if (Sub)
	{
		Sub->bCanResupply = true;

		Sub->RZDisplay(true);
	}
}

void AUBResupplyZone::OnOverlapEnd(UPrimitiveComponent * Comp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	AUBU995* Sub = Cast<AUBU995>(OtherActor);
	if (Sub)
	{
		Sub->bCanResupply = false;

		Sub->RZDisplay(false);
	}
}

