// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/Controllers/PlayerMarineController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"

void APlayerMarineController::BeginPlay()
{
	Super::BeginPlay();
	DisableInput(this);
	StartFade();
	GetWorldTimerManager().SetTimer(Timer, this, &APlayerMarineController::SetupGame, 2.0f, false);
}

void APlayerMarineController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	HUDWidget->RemoveFromViewport();
	EndFade();
	GetWorldTimerManager().SetTimer(Timer, this, &APlayerMarineController::RestartLevel, 2.0f, false);
}

void APlayerMarineController::SetupGame()
{
	if(!HUDClass) { return; }
	HUDWidget = CreateWidget(this, HUDClass);
	HUDWidget->AddToViewport();
	if(!InputEnabled()) { EnableInput(this); }
}

void APlayerMarineController::StartFade() const
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(1.0f, 0.0f, 2.0f, FColor::Black, true, false);
}

void APlayerMarineController::EndFade() const
{
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.0f, 1.0f, 2.0f, FColor::Black, true, true);
}

