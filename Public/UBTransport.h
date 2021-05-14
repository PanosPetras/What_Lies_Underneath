// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
UCLASS()
class WLU_MP_API AUBTransport : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUBTransport();

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Ship;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Container1;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Container2;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Container3;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* Container4;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* FlagOne;

	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* FlagTwo;

	UPROPERTY(BlueprintReadOnly, Category = "Sunk")
		bool sunk;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AUBLootZone> ProjectileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Overlap")
		void Overlap();

	UFUNCTION()
		void MoveDown(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
		void SpawnLootZone();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
