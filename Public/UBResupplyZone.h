// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UBResupplyZone.generated.h"

class UBoxComponent;
class USphereComponent;

UCLASS()
class WLU_MP_API AUBResupplyZone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUBResupplyZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Components")
		UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Components")
		USphereComponent* SphereComp;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* Comp, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
