// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UBDestroyer.generated.h"

class UPawnSensingComponent;
class USceneComponent;

UCLASS()
class WLU_MP_API AUBDestroyer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUBDestroyer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AUBDestroyerProjectile> Projectile;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* GunComp1;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* GunBarellComp1;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* GunComp2;

	UPROPERTY(EditAnywhere, Category = "Components")		
	UStaticMeshComponent* GunBarellComp2;

	UPROPERTY(EditAnywhere, Category = "Components")
	USkeletalMeshComponent* Flag;

	UPROPERTY(EditAnywhere, Category = "Components")		
	USceneComponent* SceneComp1;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp2;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp3;

	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* SceneComp4;

	UPROPERTY(VisibleAnywhere)
	bool sunk;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AUBLootZone> ProjectileClass;

	UPROPERTY()
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere, Category = "FX")
	UParticleSystem* FireEffect;

	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* FireSound;

	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* Sirens;

	UPROPERTY()
	bool SubSpotted;

	UFUNCTION(BlueprintImplementableEvent, Category = "Overlap")
	void Overlap();

	UFUNCTION()
	void OnHit(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnNoiseHeard(APawn* Noiseinstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnUnusedTimerEnd();

	UFUNCTION()
	void OnSpottedTimerEnd();

	FTimerHandle UnusedHandle;

	FTimerHandle SpottedHandle;

	UFUNCTION(Server, Reliable, WithValidation)
	void MoveDown(float Value);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastRPCFunction_FireEffect();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastRPCFunction_SirensSoundEffect();
};
