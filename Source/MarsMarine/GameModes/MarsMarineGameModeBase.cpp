// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/GameModes/MarsMarineGameModeBase.h"
#include "MarsMarine/Actors/PickUpSpawnPoint.h"
#include "MarsMarine/Characters/Marine.h"
#include "MarsMarine/Actors/HealthPickUp.h"
#include "MarsMarine/Characters/Spider.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/CharacterMovementComponent.h"

void AMarsMarineGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SetupGame();
	GetWorldTimerManager().SetTimer(Timer, this, &AMarsMarineGameModeBase::SpawnSpider, SpawnRate, true);
}

void AMarsMarineGameModeBase::SpiderKilled()
{
	Kills++;
	AliveSpiders--;
	if(AliveSpiders == 0)
	{
		GetWorldTimerManager().SetTimer(Timer, this, &AMarsMarineGameModeBase::SetupNewWave, 2.0f, false);
	}
}

void AMarsMarineGameModeBase::MarineKilled(AMarine* DeadMarine) const
{
	UGameplayStatics::GetPlayerController(this, 0)->GameHasEnded(DeadMarine, false);
}

int32 AMarsMarineGameModeBase::GetKills() const
{
	return Kills;
}

AActor* AMarsMarineGameModeBase::GetRandomSpawnPoint()
{
	const int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(0,  TargetPoints.Num() - 1);
	return TargetPoints[RandomInt];
}

AActor* AMarsMarineGameModeBase::GetRandomPickUpSpawnPoint()
{
	const int32 RandomInt = UKismetMathLibrary::RandomIntegerInRange(0, PickUpSpawnPoints.Num() - 1);
	return PickUpSpawnPoints[RandomInt];
}

void AMarsMarineGameModeBase::SpawnSpider()
{
	ASpider* SpawnedSpider = GetWorld()->SpawnActor<ASpider>(SpiderClass, GetRandomSpawnPoint()->GetTransform());
	SpawnedSpider->GetCharacterMovement()->MaxWalkSpeed = SpiderMovementSpeed;
	AliveSpiders++;
	RemainingSpawns--;
	if(RemainingSpawns == 0)
	{
		GetWorldTimerManager().ClearTimer(Timer);
	}
}

void AMarsMarineGameModeBase::SetupNewWave()
{
	CurrentWave++;
	SpawnLimit *= DifficultyMultiplier;
	SpawnLimit = UKismetMathLibrary::Round(SpawnLimit);
	RemainingSpawns = SpawnLimit;
	SpiderMovementSpeed *= SpiderMovementSpeedMultiplier;
	GetWorld()->SpawnActor<AHealthPickUp>(HealthPickUpClass, GetRandomPickUpSpawnPoint()->GetActorTransform());
	GetWorldTimerManager().SetTimer(Timer, this, &AMarsMarineGameModeBase::SpawnSpider, SpawnRate, true);
}

void AMarsMarineGameModeBase::SetupGame()
{
	RemainingSpawns = SpawnLimit;
	UGameplayStatics::GetAllActorsOfClass(this, ATargetPoint::StaticClass(), TargetPoints);
	UGameplayStatics::GetAllActorsOfClass(this, APickUpSpawnPoint::StaticClass(), PickUpSpawnPoints);
}

int32 AMarsMarineGameModeBase::GetCurrentWave() const
{
	return CurrentWave;
}
