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
		LeftWeapon = Bomb;
		LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWeaponSocket"));
		LeftWeapon->PickedUpCharacter = this;
	}

	AAxe* Axe = Cast<AAxe>(GetWorld()->SpawnActor(AxeClass, &SpawnLocation, &SpawnRotation));
	if (Axe)
	{
		RightWeapon = Axe;
		RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightWeaponSocket"));
		RightWeapon->PickedUpCharacter = this;
	}
}

void AMeleeEnemy::Ragdoll()
{
	if (AEnemyWeaponBTController* MeleeEnemyController = Cast<AEnemyWeaponBTController>(GetController()))
	{
		MeleeEnemyController->BrainComponent->PauseLogic("Ragdolling!");
	}
	Super::Ragdoll();
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
		ABomb* Bomb = Cast<ABomb>(GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation));
		if (Bomb)
		{
			LeftWeapon = Bomb;
			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWeaponSocket"));
			LeftWeapon->PickedUpCharacter = this;
		}
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


// Destroy with weapon or not destroying itself
// Damage TEXT
// Inventory Widget Size
// freezing weapon
// physics contraint
// boss
// level
// axe move when attack