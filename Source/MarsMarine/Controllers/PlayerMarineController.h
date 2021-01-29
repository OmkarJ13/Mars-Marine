// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerMarineController.generated.h"

UCLASS()
class MARSMARINE_API APlayerMarineController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
	void SetupGame();
	void StartFade() const;
	void EndFade() const;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY(EditAnywhere)
	float RestartRate = 2.0f;
	
	UPROPERTY()
	UUserWidget* HUDWidget;

	FTimerHandle DeathTimer;
	FTimerHandle FadeTimer;
};
