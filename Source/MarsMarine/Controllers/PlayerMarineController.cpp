// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/Controllers/PlayerMarineController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

void APlayerMarineController::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(FadeTimer, this, &APlayerMarineController::SetupGame, 1.0f, false);
	DisableInput(this);
	StartFade();
}

void APlayerMarineController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	HUDWidget->RemoveFromViewport();
	GetWorldTimerManager().SetTimer(DeathTimer, this, &APlayerMarineController::RestartLevel, RestartRate, false);
	EndFade();
}

void APlayerMarineController::SetupGame()
{
	if(!HUDClass) { return; }
	HUDWidget = CreateWidget(this, HUDClass);
	HUDWidget->AddToViewport();
	EnableInput(this);
}

void APlayerMarineController::StartFade() const
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(1.0f, 0.0f, 2.0f, FColor::Black, true, false);
}

void APlayerMarineController::EndFade() const
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.0f, 1.0f, 2.0f, FColor::Black, true, true);
}

