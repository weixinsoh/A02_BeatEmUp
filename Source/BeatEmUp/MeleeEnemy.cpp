// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"

#include "Bomb.h"
#include "BrainComponent.h"

AMeleeEnemy::AMeleeEnemy()
{
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	AActor* Bomb = GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation);
	LeftWeapon = Cast<ABomb>(Bomb);
	LeftWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWeaponSocket"));
	AActor* Axe = GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation);
	RightWeapon = Cast<AAxe>(Axe);
	RightWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightWeaponSocket"));

}

void AMeleeEnemy::Ragdoll()
{
	Super::Ragdoll();
	/*	if (AMeleeEnemyBTController* Controller = Cast<AMeleeEnemyBController>(GetController()))
	{
		Controller->BrainComponent->PauseLogic("Ragdolling!");
	}*/
}


void AMeleeEnemy::StopRagdoll()
{
	Super::StopRagdoll();
	/*	if (AMeleeEnemyBTController* Controller = Cast<AMeleeEnemyBTContoller>(GetController()))
	{
		Controller->BrainComponent->PauseLogic("Moving Again!");
	}*/

}

void AMeleeEnemy::UseLeftWeapon()
{
	LeftWeapon->UseWeapon(this);
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	AActor* Bomb = GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation);
	LeftWeapon = Cast<ABomb>(Bomb);
	LeftWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("LeftWeaponSocket"));
}

void AMeleeEnemy::UseRightWeapon()
{
	RightWeapon->UseWeapon(this);
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();
	AActor* Axe = GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation);
	RightWeapon = Cast<AAxe>(Axe);
	RightWeapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("RightWeaponSocket"));
}
