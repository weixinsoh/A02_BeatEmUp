// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bomb.h"
#include "BrainComponent.h"

AMeleeEnemy::AMeleeEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	ABomb* Bomb = Cast<ABomb>(GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation));
	if (Bomb)
	{
		UE_LOG(LogTemp, Warning, TEXT("spawned weapon"));
		LeftWeapon = Bomb;
		LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWeaponSocket"));
	}

	AAxe* Axe = Cast<AAxe>(GetWorld()->SpawnActor(AxeClass, &SpawnLocation, &SpawnRotation));
	if (Axe)
	{
		RightWeapon = Axe;
		RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightWeaponSocket")); 
	}
}

void AMeleeEnemy::Ragdoll()
{
	Super::Ragdoll();
	if (AEnemyWeaponBTController* MeleeEnemyController = Cast<AEnemyWeaponBTController>(GetController()))
	{
		MeleeEnemyController->BrainComponent->PauseLogic("Ragdolling!");
	}
}


void AMeleeEnemy::StopRagdoll()
{
	Super::StopRagdoll();
	if (AEnemyWeaponBTController* MeleeEnemyController = Cast<AEnemyWeaponBTController>(GetController()))
	{
		MeleeEnemyController->BrainComponent->ResumeLogic("Moving Again!");
	}

}

void AMeleeEnemy::UseLeftWeapon()
{
	LeftWeapon->UseWeapon(this);
	if (Cast<AEnemyWeaponBTController>(GetController())->LeftAmmo > 1)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		AActor* Bomb = GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation);
		LeftWeapon = Cast<ABomb>(Bomb);
		LeftWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWeaponSocket"));	
	}
}

void AMeleeEnemy::UseRightWeapon()
{
	RightWeapon->UseWeapon(this);
	if (Cast<AEnemyWeaponBTController>(GetController())->RightAmmo <= 1)
	{
		RightWeapon->Destroy();
	}
}

void AMeleeEnemy::SetMovementSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed += Speed;
}
