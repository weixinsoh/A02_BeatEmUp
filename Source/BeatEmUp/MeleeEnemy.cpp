// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"

#include "Bomb.h"
#include "BrainComponent.h"

AMeleeEnemy::AMeleeEnemy()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	/*
	AActor* Bomb = GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation);
	if (LeftWeapon = Cast<ABomb>(Bomb))
	{
		LeftWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWeaponSocket"));
	}
	AActor* Axe = GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation);
	RightWeapon = Cast<AAxe>(Axe);
	RightWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightWeaponSocket"));
*/
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
		MeleeEnemyController->BrainComponent->PauseLogic("Moving Again!");
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
