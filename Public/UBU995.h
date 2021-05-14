// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <string>
#include "UBU995.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class WLU_MP_API AUBU995 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUBU995();
	UPROPERTY()
	UPawnNoiseEmitterComponent* NoiseEmitter;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* Submarine;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AUBTorpedo> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	float CameraRotPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Camera")
	float CameraRotYaw;

	UFUNCTION()
	void UpdateCameraRotPitch(float Value);

	UFUNCTION()
	void UpdateCameraRotYaw(float Value);

	UPROPERTY(EditAnywhere)
	class USphereComponent* Torp;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Fire1;

	UPROPERTY(EditAnywhere)
	class USphereComponent* Fire2;

	UPROPERTY(EditAnywhere, Category = "FX")
	USoundBase* ExplosionSound;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* FlagOne;

	UPROPERTY(EditAnywhere, Category = "FX")
	UParticleSystem* FireEffect;

	UPROPERTY(EditAnywhere, Category = "FX")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* FlagTwo;

	UPROPERTY(EditAnywhere, Category = "FX")
	class USoundBase* LaunchingSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	int TypicalSpeed = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float ActualSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Depth")
	float Depth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Torpedo")
	int TorpedoCooldown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Supply")
	float TurnRate = 0;

	UPROPERTY(EditAnywhere)
	bool bCanResupply = false;

	UPROPERTY(EditAnywhere)
	bool bCanLoot = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Supply")
	int AvTorps = 14;

	UPROPERTY(BlueprintReadOnly, Category = "Supply")
	float fuel;

	UPROPERTY(EditAnywhere)
	bool bGamePaused;

	UPROPERTY(EditAnywhere)
	AActor* LootZone;
	
	UFUNCTION()
	void OnFire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float Value);

	UFUNCTION()
	void Submerge(float Value);

	UFUNCTION()
	void Turn(float Value);

	UFUNCTION()
	void OnMoveRight(float Value);

	UFUNCTION()
	void Resupply(float Value);

	UFUNCTION(BlueprintImplementableEvent, Category = "ResupplyZone")
	void RZDisplay(bool Visibility);

	UFUNCTION(BlueprintImplementableEvent, Category = "LootZone")
	void LZDisplay(bool Visibility);

	UFUNCTION(BlueprintImplementableEvent, Category = "LootZone")
	void GameOverScreen();

	FTimerHandle DestroyedHandle;

	FTimerHandle DestroyedHandle2;

	void OnHandleEnd1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCFunction_PlayEffects1();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCFunction_PlayEffects2();

	void OnHit();
};
