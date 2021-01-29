// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionArea.generated.h"

class UBoxComponent;
class AMarine;

UCLASS()
class MARSMARINE_API AMissionArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionArea();
	
private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void KillPlayer(AMarine* MarineToKill);
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* MissionArea;

	FTimerHandle KillTimer;
	FTimerDelegate KillDelegate;

	UPROPERTY(EditAnywhere)
	float KillRate = 5.0f;
};
