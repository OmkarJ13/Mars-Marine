// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Marine.generated.h"

class AGun;

UCLASS()
class MARSMARINE_API AMarine : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMarine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	float GetHealth() const;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	bool IsInMissionArea() const;
	
	void AddHealth();
	float GetMaxHealth() const;

	bool bIsInMissionArea = true;
	
private:
	UFUNCTION()
	void ReduceHealth(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void MoveForward(float Val);
	void MoveRight(float Val);
	void LMBPressed();
	void LMBReleased();
	void UpdateMouseAim() const;
	void HandleDeath();

	FVector2D GetMouseVelocity() const;
	
	bool IsMVelocityAboveDeadzone(FVector2D& MouseVelocity) const;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere)
	float MouseSensitivity = 5.0f;

	UPROPERTY(EditAnywhere)
	USoundBase* DeathSound;

	UPROPERTY()
	AGun* Gun;
	
	float Health;
	
	bool bIsShooting = false;
	bool bIsDead = false;
};
