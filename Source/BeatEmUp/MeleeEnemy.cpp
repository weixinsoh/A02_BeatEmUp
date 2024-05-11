// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Bomb.h"
#include "BrainComponent.h"

/**
 * Constructor for Melee Enemy.
 */
AMeleeEnemy::AMeleeEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

/**
 * Begin play function which spawn a bomb (as left weapon) and an axe (as right weapon) for the melee enemy.
 */
void AMeleeEnemy::BeginPlay()
{
	Super::BeginPlay();

	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = GetActorRotation();

	// Spawn a bomb and attach to the enemy's left hand socket
	ABomb* Bomb = Cast<ABomb>(GetWorld()->SpawnActor(BombClass, &SpawnLocation, &SpawnRotation));
	if (Bomb)
	{
		LeftWeapon = Bomb;
		LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                              FName("LeftWeaponSocket"));
		LeftWeapon->PickedUpCharacter = this;
	}

	// Spawn an axe and attach to the enemy's right hand socket
	AAxe* Axe = Cast<AAxe>(GetWorld()->SpawnActor(AxeClass, &SpawnLocation, &SpawnRotation));
	if (Axe)
	{
		RightWeapon = Axe;
		RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
		                               FName("RightWeaponSocket"));
		RightWeapon->PickedUpCharacter = this;
	}
}

/**
 * Perform enemy ragdolling. Pause the behaviour logic of the enemy controller.
 */
void AMeleeEnemy::Ragdoll()
{
	if (AEnemyWeaponBTController* MeleeEnemyController = Cast<AEnemyWeaponBTController>(GetController()))
	{
		MeleeEnemyController->BrainComponent->PauseLogic("Ragdolling!");
	}
	Super::Ragdoll();
}

/**
 * Stop enemy ragdolling. Resume the behaviour logic of the enemy controller.
 */
void AMeleeEnemy::StopRagdoll()
{
	Super::StopRagdoll();
	if (AEnemyWeaponBTController* MeleeEnemyController = Cast<AEnemyWeaponBTController>(GetController()))
	{
		MeleeEnemyController->BrainComponent->ResumeLogic("Moving Again!");
	}
}

/**
 * Deal damage to the player and destroy itself with its holding weapons once it hit the player.
 */
void AMeleeEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector Normal, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, Normal, Hit);
	if (OtherActor == Player)
	{
		Player->DealDamage(DamageToPlayer);
		LeftWeapon->Destroy();
		RightWeapon->Destroy();
		Destroy();
	}
}

/**
 * Perform attack action using its left weapon (bomb).
 * Spawn new bomb and attach to the enemy left hand
 * if the enemy still have left ammo after throwing the current one. 
 */
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
			LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,
			                              FName("LeftWeaponSocket"));
			LeftWeapon->PickedUpCharacter = this;
		}
	}
}

/**
 * Perform attack action using its right weapon (axe).
 * Destroy its right weapon (axe)
 * if the enemy doesn't have right ammo anymore after using the current one. 
 */
void AMeleeEnemy::UseRightWeapon()
{
	RightWeapon->UseWeapon(this);
	if (Cast<AEnemyWeaponBTController>(GetController())->RightAmmo <= 1)
	{
		RightWeapon->Destroy();
	}
}

/**
 * Set the enemy maximum walking speed.
 * @param Speed a float type number representing the new speed.
 */
void AMeleeEnemy::SetMovementSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

// Inventory details *****
// physics contraint - cannot be thrown
// boss
// level

// axe move when attack
// freezing weapon
// ticking
// folder organise
