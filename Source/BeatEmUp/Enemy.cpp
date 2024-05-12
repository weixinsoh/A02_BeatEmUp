// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "BrainComponent.h"
#include "EnemyBTController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ABeatEmUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);
	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::DealDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);

	UDamageTextUI* SpawnedDamage = Cast<UDamageTextUI>(CreateWidget(GetGameInstance(), DamageTextClass));
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), GetMesh()->GetComponentLocation(), SpawnedDamage->CurrentLocation);
	SpawnedDamage->DamageText->SetText(FText::FromString(FString::SanitizeFloat(Damage)));
	SpawnedDamage->TargetLocation += SpawnedDamage->CurrentLocation;
	SpawnedDamage->AddToViewport();
}

void AEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	if (OtherActor == Player)
	{
		Player->DealDamage(DamageToPlayer);
		Player->NumEnemiesDefeated += 1;
		Destroy();
	}
}

void AEnemy::Ragdoll()
{
	if (AEnemyBTController* EnemyBTController = Cast<AEnemyBTController>(GetController()))
	{
		EnemyBTController->BrainComponent->PauseLogic("Ragdolling!");
	}
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetWorld()->GetTimerManager().SetTimer(RagdollTimerHandle, this, &AEnemy::StopRagdoll, RagdollTime, false);

}

void AEnemy::StopRagdoll()
{
	if (CurrentHealth <= 0 && Player)
	{
		Player->NumEnemiesDefeated += 1;
		Player->AddEXP(EXPAmount);
		Destroy();
		return;
	}
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetCollisionProfileName("CharacterMesh");
	GetCapsuleComponent()->SetWorldLocation(GetMesh()->GetSocketLocation("pelvis"));
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	GetCapsuleComponent()->SetCollisionProfileName("Pawn");
	if (AEnemyBTController* EnemyBTController = Cast<AEnemyBTController>(GetController()))
	{
		EnemyBTController->BrainComponent->ResumeLogic("Moving Again!");
	}
	GetCharacterMovement()->GravityScale = 1;
}

/**
 * This function diables enemy movement and
 * pause the behaviour logic of the enemy controller.
 */
void AEnemy::Freeze()
{
	if (AEnemyBTController* EnemyBTController = Cast<AEnemyBTController>(GetController()))
	{
		EnemyBTController->BrainComponent->PauseLogic("Freezing!");
	}
	GetCharacterMovement()->DisableMovement();
	GetWorld()->GetTimerManager().SetTimer(FreezeTimeHandle, this, &AEnemy::UnFreeze, FreezeTime, false);
}

/**
 * This function enables back enemy movement and
 * resumes the behaviour logic of the enemy controller.
 */
void AEnemy::UnFreeze()
{
	if (CurrentHealth <= 0 && Player)
	{
		Player->NumEnemiesDefeated += 1;
		Destroy();
		return;
	}
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	if (AEnemyBTController* EnemyBTController = Cast<AEnemyBTController>(GetController()))
	{
		EnemyBTController->BrainComponent->ResumeLogic("Moving Again!");
	}
}

void AEnemy::Shoot(FVector Direction)
{
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100;
	FRotator SpawnRotation = Direction.Rotation();
	ABullet* SpawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(BulletClass, &SpawnLocation, &SpawnRotation));
	Direction.Normalize();
	SpawnedBullet->MovementComponent->Velocity = Direction * SpawnedBullet->MovementSpeed;
}

