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
	void StartFade() const;
	void EndFade() const;
	void SetupGame();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;
	
	UPROPERTY()
	UUserWidget* HUDWidget;

	FTimerHandle Timer;
};
