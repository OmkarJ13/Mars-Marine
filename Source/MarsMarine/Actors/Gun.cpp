// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/Actors/Gun.h"
#include "MarsMarine/Characters/Spider.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Component"));
	Mesh->SetupAttachment(Root);

	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Muzzle Flash"));
	MuzzleFlash->SetupAttachment(Mesh, FName("MuzzleFlashSocket"));
	MuzzleFlash->SetAutoActivate(false);

	ShotSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Shot Sound"));
	ShotSound->SetupAttachment(Mesh, FName("MuzzleFlashSocket"));
	ShotSound->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::StartFiring()
{
	MuzzleFlash->Activate();
	ShotSound->Play();
	GetWorldTimerManager().SetTimer(TraceTimer, this, &AGun::GunTrace, FireRate, true);
}

void AGun::StopFiring()
{
	GetWorldTimerManager().ClearTimer(TraceTimer);
	MuzzleFlash->Deactivate();
	ShotSound->Stop();
	UGameplayStatics::SpawnSound2D(this, ShotEnd);
}

void AGun::GunTrace()
{
	FHitResult Hit;
	const FVector Start = Mesh->GetSocketLocation(FName("MuzzleFlashSocket"));
	const FVector End = Start + GetOwner()->GetActorForwardVector() * 1000.0f;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	AActor* HitActor = Hit.GetActor();
	if(HitActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, Hit.ImpactPoint);
		if(HitActor->IsA(ASpider::StaticClass()))
		{
			UGameplayStatics::ApplyDamage(HitActor, DamageAmount, GetOwner()->GetInstigatorController(), this, nullptr);
		}
	}
}


