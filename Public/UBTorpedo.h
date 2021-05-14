// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "UBTorpedo.generated.h"

UCLASS()
class WLU_MP_API AUBTorpedo : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AUBTorpedo();

	//Declares the torpedo skeletal mesh
	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* Torpedo;

	//Declares the collision sphere
	UPROPERTY(EditAnywhere)
	class USphereComponent* _collision;

	UPROPERTY(EditAnywhere, Category = "FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Radius = 500.f;

	// Called When the torpedo hits something
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastRPCFunction_PlayExplosionEffect();

	void MoveForward(float Value);

};
