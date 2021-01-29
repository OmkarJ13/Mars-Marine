// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/Characters/Marine.h"
#include "MarsMarine/Actors/Gun.h"
#include "MarsMarine/GameModes/MarsMarineGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMarine::AMarine()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AMarine::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &AMarine::ReduceHealth);
	Health = MaxHealth;
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponPoint"));
	Gun->SetOwner(this);
}

// Called every frame
void AMarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bIsDead)
	{
		UpdateMouseAim();
	}
}

// Called to bind functionality to input
void AMarine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMarine::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMarine::MoveRight);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AMarine::LMBPressed);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Released, this, &AMarine::LMBReleased);
}

float AMarine::GetHealth() const
{
	return Health;
}

bool AMarine::IsDead() const
{
	return bIsDead;
}

bool AMarine::IsInMissionArea() const
{
	return bIsInMissionArea;
}

void AMarine::MoveForward(float Val)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Val);
}

void AMarine::MoveRight(float Val)
{
	AddMovementInput(FVector(0.0f, 1.0f, 0.0f), Val);
}

void AMarine::LMBPressed()
{
	bIsShooting = true;
	Gun->StartFiring();
}

void AMarine::LMBReleased()
{
	bIsShooting = false;
	Gun->StopFiring();
}

FVector2D AMarine::GetMouseVelocity() const
{
	float DeltaX;
	float DeltaY;
	UGameplayStatics::GetPlayerController(this, 0)->GetInputMouseDelta(DeltaX, DeltaY);
	const FVector2D MouseMovement = {DeltaX, DeltaY};
	return MouseMovement / GetWorld()->GetDeltaSeconds();
}

bool AMarine::IsMVelocityAboveDeadzone(FVector2D& MouseVelocity) const
{
	if(MouseVelocity.Size() > 20.0f)
	{
		return true;
	}
	return false;
}

void AMarine::UpdateMouseAim() const
{
	FVector2D MouseVelocity = GetMouseVelocity();
	if(IsMVelocityAboveDeadzone(MouseVelocity))
	{
		APlayerController* MarineController = UGameplayStatics::GetPlayerController(this, 0);
		const float Yaw = atan2f(MouseVelocity.X, MouseVelocity.Y);
		const float YawDegrees = Yaw * 180.f / 3.141592f;
		const FRotator TargetRotation = {0.f, YawDegrees, 0.f};
		const FRotator StartRotation = Controller->GetControlRotation();
		const FRotator SmoothRotation = FMath::RInterpTo(StartRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), MouseSensitivity);
		MarineController->SetControlRotation(SmoothRotation);
	}
}

void AMarine::HandleDeath()
{
	bIsDead = true;
	UGameplayStatics::SpawnSound2D(this, DeathSound);
	DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	if(bIsShooting) { Gun->StopFiring(); }
	AMarsMarineGameModeBase* GameMode = Cast<AMarsMarineGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameMode->MarineKilled(this);
}

void AMarine::AddHealth()
{
	Health = MaxHealth;
}

float AMarine::GetMaxHealth() const
{
	return MaxHealth;
}

void AMarine::ReduceHealth(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(bIsDead)
	{
		return;
	}
	Health -= Damage;
	Health = FMath::Clamp(Health, 0.0f, 100.0f);
	if(Health == 0.0f)
	{
		HandleDeath();
	}
}


