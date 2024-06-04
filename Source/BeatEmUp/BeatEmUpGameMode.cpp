// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeatEmUpGameMode.h"
#include "BeatEmUpCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BeatEmUpGameInstance.h"
#include "BossBTController.h"
#include "BossEnemy.h"
#include "DynamicColorFloor.h"
#include "EnemyBTController.h"
#include "Flail.h"
#include "MeleeEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ABeatEmUpGameMode::ABeatEmUpGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ABeatEmUpGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ABeatEmUpGameMode::PostBeginPlay);
}

/**
 * Load the game by setting back to the previous saved state.
 */
void ABeatEmUpGameMode::Load(UBeatEmUpSaveGame* LoadedGame)
{
	// Set the player character to the previous saved state
	ABeatEmUpCharacter* PlayerCharacter = Cast<ABeatEmUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerCharacter->SetActorLocationAndRotation(LoadedGame->PlayerPosition, LoadedGame->PlayerRotation);
	PlayerCharacter->PunchDamage = LoadedGame->PlayerPunchDamage;
	PlayerCharacter->CurrentHealth = LoadedGame->PlayerCurrentHealth;
	PlayerCharacter->MaxHealth = LoadedGame->PlayerMaxHealth;
	PlayerCharacter->CurrentEXP = LoadedGame->PlayerCurrentEXP;
	PlayerCharacter->EXPToLevel = LoadedGame->PlayerEXPToLevel;
	PlayerCharacter->InGameUI->UpdateValues();
	PlayerCharacter->NumEnemiesDefeated = LoadedGame->PlayerNumOfEnemiesDefeated;

	// Add the previous collected weapon back to the inventory
	PlayerCharacter->Inventory = LoadedGame->PlayerInventory;
	PlayerCharacter->InventoryWidget->RefreshInventory(PlayerCharacter->Inventory);
	for (AWeapon* Weapon: PlayerCharacter->Inventory)
	{
		Weapon->SetActorHiddenInGame(true);
		Weapon->PickedUpCharacter = PlayerCharacter;
	}

	// Set the player character to equip previous equipping weapon
	if (LoadedGame->PlayerEquippingWeapon != nullptr)
	{
		PlayerCharacter->EquippingWeapon = LoadedGame->PlayerEquippingWeapon;
		PlayerCharacter->EquippingWeapon->SetActorHiddenInGame(false);
		if (Cast<AFlail>(PlayerCharacter->EquippingWeapon))
		{
			return;
		}
		FName WeaponSocket = FName("WeaponSocket");
		PlayerCharacter->EquippingWeapon->AttachToComponent(PlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	}

	// Set the enemies to the previous saved state
	for (int i = 0; i < LoadedGame->EnemyLocations.Num(); i++)
	{
		AEnemy* SpawnedEnemy = Cast<AEnemy>(GetWorld()->SpawnActor(EnemyClass));

		if (SpawnedEnemy)
		{
			SpawnedEnemy->SetActorLocationAndRotation(LoadedGame->EnemyLocations[i], LoadedGame->EnemyRotations[i]);
			SpawnedEnemy->CurrentHealth = LoadedGame->EnemyCurrentHealths[i];
			SpawnedEnemy->MaxHealth = LoadedGame->EnemyMaxHealths[i];
			if (LoadedGame->EnemyRagdollStates[i])
			{
				SpawnedEnemy->Ragdoll();
				SpawnedEnemy->GetCharacterMovement()->GravityScale = 0;
				SpawnedEnemy->GetMesh()->SetWorldLocation(LoadedGame->EnemyMeshLocations[i], false, nullptr,
														  ETeleportType::TeleportPhysics);
				SpawnedEnemy->GetMesh()->SetAllPhysicsLinearVelocity(LoadedGame->EnemyMeshVelocities[i], true);
			}
			// Set the ammo to the previous amount
			AEnemyBTController* SpawnedEnemyController = Cast<AEnemyBTController>(SpawnedEnemy->GetController());
			if (SpawnedEnemyController)
			{
				SpawnedEnemyController->Ammo = LoadedGame->EnemyAmmo[i];
			}
		}
	}

	// Destroy all the melee enemies spawned by default
	TArray<AActor*> MeleeEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeleeEnemy::StaticClass(), MeleeEnemies);
	for (int i = 0; i < MeleeEnemies.Num(); i++)
	{
		AMeleeEnemy* SpawnedMeleeEnemy = Cast<AMeleeEnemy>(MeleeEnemies[i]);
		SpawnedMeleeEnemy->Destroy();
		SpawnedMeleeEnemy->LeftWeapon->Destroy();
		SpawnedMeleeEnemy->RightWeapon->Destroy();
	}

	// Set the melee enemies to the previous saved state
	for (int i = 0; i < LoadedGame->MeleeEnemyLocations.Num(); i++){
		AMeleeEnemy* SpawnedMeleeEnemy = Cast<AMeleeEnemy>(GetWorld()->SpawnActor(MeleeEnemyClass));
		if (SpawnedMeleeEnemy)
		{
			SpawnedMeleeEnemy->SetActorLocationAndRotation(LoadedGame->MeleeEnemyLocations[i], LoadedGame->MeleeEnemyRotations[i]);
			SpawnedMeleeEnemy->CurrentHealth = LoadedGame->MeleeEnemyCurrentHealths[i];
			SpawnedMeleeEnemy->MaxHealth = LoadedGame->MeleeEnemyMaxHealths[i];
			SpawnedMeleeEnemy->GetCharacterMovement()->MaxWalkSpeed = LoadedGame->MeleeEnemyMaxWalkspeed[i];

			if (LoadedGame->MeleeEnemyRagdollStates[i])
			{
				SpawnedMeleeEnemy->Ragdoll();
				SpawnedMeleeEnemy->GetCharacterMovement()->GravityScale = 0;
				SpawnedMeleeEnemy->GetMesh()->SetWorldLocation(LoadedGame->MeleeEnemyMeshLocations[i], false, nullptr,
														  ETeleportType::TeleportPhysics);
				SpawnedMeleeEnemy->GetMesh()->SetAllPhysicsLinearVelocity(LoadedGame->MeleeEnemyMeshVelocities[i], true);
			}
			// Set the ammo to the previous amount 
			AEnemyWeaponBTController* SpawnedMeleeEnemyController = Cast<AEnemyWeaponBTController>(SpawnedMeleeEnemy->GetController());
			if (SpawnedMeleeEnemyController)
			{
				SpawnedMeleeEnemyController->LeftAmmo = LoadedGame->MeleeEnemyLeftAmmo[i];
				SpawnedMeleeEnemyController->RightAmmo = LoadedGame->MeleeEnemyRightAmmo[i];
				if (SpawnedMeleeEnemyController->LeftAmmo <= 0)
				{
					SpawnedMeleeEnemy->LeftWeapon->Destroy();
				} 
				if (SpawnedMeleeEnemyController->RightAmmo <= 0)
				{
					SpawnedMeleeEnemy->RightWeapon->Destroy();
				}
			}
		}
	}

	// Set the boss enemy to the previous state
	ABossEnemy* SpawnedBossEnemy = Cast<ABossEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossEnemy::StaticClass()));
	if (SpawnedBossEnemy)
	{
		SpawnedBossEnemy->SetActorLocationAndRotation(LoadedGame->BossEnemyLocation, LoadedGame->BossEnemyRotation);
		SpawnedBossEnemy->CurrentHealth = LoadedGame->BossEnemyCurrentHealth;
		SpawnedBossEnemy->MaxHealth = LoadedGame->BossEnemyMaxHealth;
		SpawnedBossEnemy->BossUI->UpdateValues();
		if (LoadedGame->BossEnemyRagdollState)
		{
			SpawnedBossEnemy->Ragdoll();
			SpawnedBossEnemy->GetCharacterMovement()->GravityScale = 0;
			SpawnedBossEnemy->GetMesh()->SetWorldLocation(LoadedGame->BossEnemyMeshLocation, false, nullptr,
													  ETeleportType::TeleportPhysics);
			SpawnedBossEnemy->GetMesh()->SetAllPhysicsLinearVelocity(LoadedGame->BossEnemyMeshVelocity, true);
		}
		// Set the ammo to the previous amount
		ABossBTController* SpawnedBossEnemyController = Cast<ABossBTController>(SpawnedBossEnemy->GetController());
		if (SpawnedBossEnemyController)
		{
			SpawnedBossEnemyController->bIsLeftChildDefeated = LoadedGame->bIsLeftChildDefeated;
			SpawnedBossEnemyController->bIsRightChildDefeated = LoadedGame->bIsRightChildDefeated;
			if (!SpawnedBossEnemyController->bIsLeftChildDefeated || !SpawnedBossEnemyController->bIsRightChildDefeated)
			{
				SpawnedBossEnemyController->BlackboardComponent->SetValueAsBool("SpawnEnemy", false);
			}
		}
	}

	// Set the bullet shot by the enemy to the previous state
	for (int i = 0; i < LoadedGame->BulletLocations.Num(); i++)
	{
		ABullet* SpawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(BulletClass));
		if (SpawnedBullet)
		{
			SpawnedBullet->SetActorLocation(LoadedGame->BulletLocations[i]);
			SpawnedBullet->SetActorRotation(LoadedGame->BulletRotations[i]);
		}
	}

	// Set the bomb thrown by the melee enemy to the previous state
	for (int i = 0; i < LoadedGame->BombLocations.Num(); i++)
	{
		ABomb* SpawnedBomb = Cast<ABomb>(GetWorld()->SpawnActor(BombClass));
		if (SpawnedBomb)
		{
			SpawnedBomb->SetActorLocation(LoadedGame->BombLocations[i]);
			SpawnedBomb->BombMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SpawnedBomb->BombMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
			SpawnedBomb->BombMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

			SpawnedBomb->BombMesh->SetSimulatePhysics(true);
			SpawnedBomb->BombMesh->SetPhysicsLinearVelocity(LoadedGame->BombVelocities[i]);
			SpawnedBomb->BombMesh->SetEnableGravity(true);
			SpawnedBomb->BombMesh->SetNotifyRigidBodyCollision(true);
			SpawnedBomb->BombMesh->OnComponentHit.AddDynamic(SpawnedBomb, &ABomb::OnHit);
		}
	}
	// Set the dynamic floors to the previous color
	TArray<AActor*> DynamicFloors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADynamicColorFloor::StaticClass(), DynamicFloors);
	for (int i = 0; i < LoadedGame->DynamicFloorColors.Num(); i++)
	{
		ADynamicColorFloor* CurrentFloor = Cast<ADynamicColorFloor>(DynamicFloors[i]);
		CurrentFloor->EmissiveMaterialInstance->SetVectorParameterValue(FName("BaseColor"), LoadedGame->DynamicFloorColors[i]);
	}
}

void ABeatEmUpGameMode::Save(UBeatEmUpSaveGame* SaveGame)
{
	// Save the player character's state
	ABeatEmUpCharacter* PlayerCharacter = Cast<ABeatEmUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	SaveGame->PlayerPosition = PlayerCharacter->GetActorLocation();
	SaveGame->PlayerRotation = PlayerCharacter->GetActorRotation();
	SaveGame->PlayerCurrentHealth = PlayerCharacter->CurrentHealth;
	SaveGame->PlayerMaxHealth = PlayerCharacter->MaxHealth;
	SaveGame->PlayerPunchDamage = PlayerCharacter->PunchDamage;
	SaveGame->PlayerCurrentEXP = PlayerCharacter->CurrentEXP;
	SaveGame->PlayerEXPToLevel = PlayerCharacter->EXPToLevel;
	SaveGame->PlayerInventory = PlayerCharacter->Inventory;
	SaveGame->PlayerEquippingWeapon = PlayerCharacter->EquippingWeapon;
	SaveGame->PlayerNumOfEnemiesDefeated = PlayerCharacter->NumEnemiesDefeated;

	// Save the enemies' state
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
	for (int i = 0; i < Enemies.Num(); i++)
	{
		AMeleeEnemy* CurrentMeleeEnemy = Cast<AMeleeEnemy>(Enemies[i]);
		ABossEnemy* CurrentBossEnemy = Cast<ABossEnemy>(Enemies[i]);
		// Save the melee enemies' state
		if (CurrentMeleeEnemy)
		{
			SaveGame->MeleeEnemyLocations.Add(CurrentMeleeEnemy->GetActorLocation());
			SaveGame->MeleeEnemyRotations.Add(CurrentMeleeEnemy->GetActorRotation());
			SaveGame->MeleeEnemyCurrentHealths.Add(CurrentMeleeEnemy->CurrentHealth);
			SaveGame->MeleeEnemyMaxHealths.Add(CurrentMeleeEnemy->MaxHealth);
			if (CurrentMeleeEnemy->GetMesh()->GetCollisionProfileName() == "Ragdoll")
			{
				SaveGame->MeleeEnemyRagdollStates.Add(true);
			}
			else
			{
				SaveGame->MeleeEnemyRagdollStates.Add(false);
			}
			SaveGame->MeleeEnemyMeshLocations.Add(CurrentMeleeEnemy->GetMesh()->GetComponentLocation());
			SaveGame->MeleeEnemyMeshVelocities.Add(CurrentMeleeEnemy->GetMesh()->GetComponentVelocity());
			SaveGame->MeleeEnemyMaxWalkspeed.Add(CurrentMeleeEnemy->GetCharacterMovement()->MaxWalkSpeed);
			if (ABomb* Bomb = Cast<ABomb>(CurrentMeleeEnemy->LeftWeapon))
			{
				SaveGame->MeleeEnemyLeftWeaponLocations.Add(Bomb->BombMesh->GetComponentLocation());
				SaveGame->MeleeEnemyLeftWeaponVelocities.Add(Bomb->BombMesh->GetComponentVelocity());
			}
			AEnemyWeaponBTController* EnemyWeaponBTController = Cast<AEnemyWeaponBTController>(CurrentMeleeEnemy->GetController());
			SaveGame->MeleeEnemyLeftAmmo.Add(EnemyWeaponBTController->LeftAmmo);
			SaveGame->MeleeEnemyRightAmmo.Add(EnemyWeaponBTController->RightAmmo);
		} else if (CurrentBossEnemy)
		{
			// Save the boss enemy state
			SaveGame->BossEnemyLocation = CurrentBossEnemy->GetActorLocation();
			SaveGame->BossEnemyRotation = CurrentBossEnemy->GetActorRotation();
			SaveGame->BossEnemyCurrentHealth = CurrentBossEnemy->CurrentHealth;
			SaveGame->BossEnemyMaxHealth = CurrentBossEnemy->MaxHealth;
			if (CurrentBossEnemy->GetMesh()->GetCollisionProfileName() == "Ragdoll")
			{
				SaveGame->BossEnemyRagdollState = true;
			}
			else
			{
				SaveGame->BossEnemyRagdollState = false;
			}
			SaveGame->BossEnemyMeshLocation = CurrentBossEnemy->GetMesh()->GetComponentLocation();
			SaveGame->BossEnemyMeshVelocity = CurrentBossEnemy->GetMesh()->GetComponentVelocity();
			ABossBTController* EnemyBTController = Cast<ABossBTController>(CurrentBossEnemy->GetController());
			SaveGame->bIsLeftChildDefeated = EnemyBTController->bIsLeftChildDefeated;
			SaveGame->bIsRightChildDefeated = EnemyBTController->bIsRightChildDefeated;
		} else
		{
			// Save the basic enemies' state
			AEnemy* CurrentEnemy = Cast<AEnemy>(Enemies[i]);
			SaveGame->EnemyLocations.Add(CurrentEnemy->GetActorLocation());
			SaveGame->EnemyRotations.Add(CurrentEnemy->GetActorRotation());
			SaveGame->EnemyCurrentHealths.Add(CurrentEnemy->CurrentHealth);
			SaveGame->EnemyMaxHealths.Add(CurrentEnemy->MaxHealth);
			if (CurrentEnemy->GetMesh()->GetCollisionProfileName() == "Ragdoll")
			{
				SaveGame->EnemyRagdollStates.Add(true);
			}
			else
			{
				SaveGame->EnemyRagdollStates.Add(false);
			}
			SaveGame->EnemyMeshLocations.Add(CurrentEnemy->GetMesh()->GetComponentLocation());
			SaveGame->EnemyMeshVelocities.Add(CurrentEnemy->GetMesh()->GetComponentVelocity());
			AEnemyBTController* EnemyBTController = Cast<AEnemyBTController>(CurrentEnemy->GetController());
			SaveGame->EnemyAmmo.Add(EnemyBTController->Ammo);
		}
	}

	// Save the state of the bombs being thrown
	TArray<AActor*> Bombs;
	TArray<AActor*> MeleeEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABomb::StaticClass(), Bombs);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeleeEnemy::StaticClass(), MeleeEnemies);
	for (int i = 0; i < Bombs.Num(); i++)
	{
		bool bIsAttached = false;
		for (int j = 0; j < MeleeEnemies.Num(); j++)
		{
			if (Bombs[i]->IsAttachedTo(MeleeEnemies[j]))
			{
				bIsAttached = true;
			}

		}
		if (!bIsAttached)
		{
			ABomb* CurrentBomb = Cast<ABomb>(Bombs[i]);
			SaveGame->BombLocations.Add(CurrentBomb->BombMesh->GetComponentLocation());
			SaveGame->BombVelocities.Add(CurrentBomb->BombMesh->GetComponentVelocity());
		}

	}

	// Save the state of the bullets being shot
	TArray<AActor*> Bullets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABullet::StaticClass(), Bullets);
	for (int i = 0; i < Bullets.Num(); i++)
	{
		ABullet* CurrentBullet = Cast<ABullet>(Bullets[i]);
		SaveGame->BulletLocations.Add(CurrentBullet->GetActorLocation());
		SaveGame->BulletRotations.Add(CurrentBullet->GetActorRotation());
	}

	// Save the state of the dynamic floors
	TArray<AActor*> DynamicColorFloors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADynamicColorFloor::StaticClass(), DynamicColorFloors);
	for (int i = 0; i < DynamicColorFloors.Num(); i++)
	{
		ADynamicColorFloor* CurrentFloor = Cast<ADynamicColorFloor>(DynamicColorFloors[i]);
		FLinearColor RetrievedColor;
		CurrentFloor->EmissiveMaterialInstance->GetVectorParameterValue(FName("BaseColor"), RetrievedColor);
		SaveGame->DynamicFloorColors.Add(RetrievedColor);
	}

}

void ABeatEmUpGameMode::PostBeginPlay()
{
	UBeatEmUpGameInstance* GameInstance = Cast<UBeatEmUpGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		if (GameInstance->bLoadSave)
		{
			UBeatEmUpSaveGame* LoadedGame = Cast<UBeatEmUpSaveGame>(UGameplayStatics::LoadGameFromSlot("TestSave", 0));
			if (LoadedGame)
			{
				Load(LoadedGame);
			}
		}
	}
}
