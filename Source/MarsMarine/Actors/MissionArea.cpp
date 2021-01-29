// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/Actors/MissionArea.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MarsMarine/Characters/Marine.h"

// Sets default values
AMissionArea::AMissionArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MissionArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Mission Area"));
	RootComponent = MissionArea;
	MissionArea->OnComponentBeginOverlap.AddDynamic(this, &AMissionArea::OnOverlapBegin);
	MissionArea->OnComponentEndOverlap.AddDynamic(this, &AMissionArea::OnOverlapEnd);
}

void AMissionArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if(AMarine* Marine = Cast<AMarine>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Begin Overlap"));
		Marine->bIsInMissionArea = true;
		GetWorldTimerManager().ClearTimer(KillTimer);
		KillDelegate.Unbind();
	}
}

void AMissionArea::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if(AMarine* Marine = Cast<AMarine>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("End Overlap"));
		Marine->bIsInMissionArea = false;
		KillDelegate.BindUFunction(this, FName("KillPlayer"), Marine);
		GetWorldTimerManager().SetTimer(KillTimer, KillDelegate, KillRate, false);
	}
}

void AMissionArea::KillPlayer(AMarine* MarineToKill)
{
	UGameplayStatics::ApplyDamage(MarineToKill, MarineToKill->GetMaxHealth(), GetInstigatorController(), this, nullptr);
}



