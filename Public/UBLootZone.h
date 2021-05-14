// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UBLootZone.generated.h"

class UBoxComponent;


UCLASS()
class WLU_MP_API AUBLootZone : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUBLootZone();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComp;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* Comp, AActor * OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
