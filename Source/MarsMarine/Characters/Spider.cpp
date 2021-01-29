// Fill out your copyright notice in the Description page of Project Settings.

#include "MarsMarine/Characters/Spider.h"
#include "MarsMarine/Controllers/EnemyAIController.h"
#include "MarsMarine/GameModes/MarsMarineGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpider::ASpider()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpider::BeginPlay()
{
	Super::BeginPlay();
	OnTakeAnyDamage.AddDynamic(this, &ASpider::ReduceHealth);
	AIController = Cast<AAIController>(GetController());
	Health = MaxHealth;
}

// Called every frame
void ASpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bIsDead)
	{
		if(FollowPlayer() && !IsAttacking())
		{
			SetupAndPlayMontage();
		}
	}
}

void ASpider::DamagePlayer()
{
	UGameplayStatics::ApplyDamage(UGameplayStatics::GetPlayerCharacter(this, 0), DamageAmount, GetInstigatorController(), this, nullptr);
}

bool ASpider::IsDead() const
{
	return bIsDead;
}

void ASpider::ReduceHealth(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
                           AActor* DamageCauser)
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

bool ASpider::IsAttacking() const
{
	if(GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage))
	{
		return true;
	}
	return false;
}

bool ASpider::FollowPlayer() const
{
	const EPathFollowingRequestResult::Type Result = AIController->MoveToActor(UGameplayStatics::GetPlayerPawn(this, 0), AttackRange);
	if(Result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return true;
	}
	return false;
}

void ASpider::ActivateMovement(class UAnimMontage* Montage, bool bInterrupted)
{
	GetCharacterMovement()->Activate();
}

void ASpider::DeactivateMovement() const
{
	GetCharacterMovement()->Deactivate();
}

void ASpider::SetupAndPlayMontage()
{
	DeactivateMovement();
	PlayAnimMontage(AttackMontage);
	FOnMontageEnded OnCompleted;
	OnCompleted.BindUFunction(this, FName("ActivateMovement"));
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnCompleted);
}

void ASpider::Despawn()
{
	Destroy();
}

void ASpider::HandleDeath()
{
	bIsDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(DeathTimer, this, &ASpider::Despawn, DespawnRate, false);
	AMarsMarineGameModeBase* GameMode = Cast<AMarsMarineGameModeBase>(UGameplayStatics::GetGameMode(this));
	GameMode->SpiderKilled();
}


