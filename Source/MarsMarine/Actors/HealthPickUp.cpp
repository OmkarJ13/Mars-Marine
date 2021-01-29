// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/Actors/HealthPickUp.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "MarsMarine/Characters/Marine.h"

// Sets default values
AHealthPickUp::AHealthPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	SetRootComponent(Collider);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickUp::OnBeginOverlap);
	
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Pick Up Effect"));
	Effect->SetupAttachment(Collider);
}

void AHealthPickUp::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMarine* PlayerMarine = Cast<AMarine>(OtherActor);
	if(PlayerMarine)
	{
		if(PlayerMarine->GetHealth() == PlayerMarine->GetMaxHealth())
		{
			return;
		}
		
		PlayerMarine->AddHealth();
		UGameplayStatics::SpawnSound2D(this, PickUpSound);
		Destroy();
	}
}


