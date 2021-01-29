// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class MARSMARINE_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void StartFiring();
	void StopFiring();
	
private:
	void GunTrace();
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* ShotSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ShotEnd;

	UPROPERTY(EditAnywhere)
	float DamageAmount = 15.0f;

	UPROPERTY(EditAnywhere)
	float FireRate = 0.08f;
	
	FTimerHandle TraceTimer;
};
