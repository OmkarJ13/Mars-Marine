// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MarsMarineGameModeBase.generated.h"

class ATargetPoint;
class ASpider;
class AHealthPickUp;
class AMarine;

UCLASS()
class MARSMARINE_API AMarsMarineGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void SpiderKilled();
	void MarineKilled(AMarine* DeadMarine) const;

	UFUNCTION(BlueprintPure)
	int32 GetKills() const;
	
	UFUNCTION(BlueprintPure)
	int32 GetCurrentWave() const;

protected:
	virtual void BeginPlay() override;
	
private:
	AActor* GetRandomSpawnPoint();
	AActor* GetRandomPickUpSpawnPoint();
	
	void SpawnSpider();
	void SetupNewWave();
	void SetupGame();
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASpider> SpiderClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHealthPickUp> HealthPickUpClass;

	UPROPERTY(EditAnywhere)
	float SpawnLimit = 5.0f;

	UPROPERTY(EditAnywhere)
	float SpawnRate = 1.5f;
	
	UPROPERTY(EditAnywhere)
	float DifficultyMultiplier = 1.5f;

	UPROPERTY(EditAnywhere)
	float SpiderMovementSpeed = 300.0f;

	UPROPERTY(EditAnywhere)
	float SpiderMovementSpeedMultiplier = 1.05f;

	int32 Kills = 0;
	int32 CurrentWave = 1;
	int32 RemainingSpawns;
	int32 AliveSpiders = 0;
	
	FTimerHandle Timer;

	UPROPERTY()
	TArray<AActor*> TargetPoints;
	
	UPROPERTY()
	TArray<AActor*> PickUpSpawnPoints;
};
