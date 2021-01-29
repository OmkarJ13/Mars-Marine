// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Spider.generated.h"

class AAIController;

UCLASS()
class MARSMARINE_API ASpider : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpider();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DamagePlayer();

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

private:
	UFUNCTION()
    void ReduceHealth(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
    void ActivateMovement(UAnimMontage* Montage, bool bInterrupted);
	
	bool IsAttacking() const;
	bool FollowPlayer() const;
	
	void DeactivateMovement() const;
	void SetupAndPlayMontage();
	void Despawn();
	void HandleDeath();
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	float AttackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageAmount = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
	float DespawnRate = 3.0f;
	
	UPROPERTY()
	AAIController* AIController;

	float Health;
	
	bool bIsDead = false;
	
	FTimerHandle DeathTimer;
};
