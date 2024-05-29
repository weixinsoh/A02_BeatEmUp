// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeatEmUpGameMode.h"
#include "BeatEmUpCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BeatEmUpGameInstance.h"
#include "DynamicColorFloor.h"
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

void ABeatEmUpGameMode::Load(UBeatEmUpSaveGame* LoadedGame)
{
	ABeatEmUpCharacter* PlayerCharacter = Cast<ABeatEmUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerCharacter->SetActorLocationAndRotation(LoadedGame->PlayerPosition, LoadedGame->PlayerRotation);
	PlayerCharacter->PunchDamage = LoadedGame->PlayerPunchDamage;
	PlayerCharacter->CurrentHealth = LoadedGame->PlayerCurrentHealth;
	PlayerCharacter->MaxHealth = LoadedGame->PlayerMaxHealth;
	PlayerCharacter->CurrentEXP = LoadedGame->PlayerCurrentEXP;
	PlayerCharacter->EXPToLevel = LoadedGame->PlayerEXPToLevel;
	PlayerCharacter->InGameUI->UpdateValues();
	PlayerCharacter->NumEnemiesDefeated = LoadedGame->PlayerNumOfEnemiesDefeated;
	PlayerCharacter->Inventory = LoadedGame->PlayerInventory;
	PlayerCharacter->InventoryWidget->RefreshInventory(PlayerCharacter->Inventory);
	for (AWeapon* Weapon: PlayerCharacter->Inventory)
	{
		Weapon->SetActorHiddenInGame(true);
		Weapon->PickedUpCharacter = PlayerCharacter;
	}
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
		}
	}
	TArray<AActor*> MeleeEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMeleeEnemy::StaticClass(), MeleeEnemies);
	for (int i = 0; i < MeleeEnemies.Num(); i++)
	{
		AMeleeEnemy* SpawnedMeleeEnemy = Cast<AMeleeEnemy>(MeleeEnemies[i]);
		SpawnedMeleeEnemy->Destroy();
		SpawnedMeleeEnemy->LeftWeapon->Destroy();
		SpawnedMeleeEnemy->RightWeapon->Destroy();
	}
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
			
			AEnemyWeaponBTController* SpawnedMeleeEnemyController = Cast<AEnemyWeaponBTController>(SpawnedMeleeEnemy->GetController());
			if (SpawnedMeleeEnemyController)
			{
				SpawnedMeleeEnemyController->LeftAmmo = LoadedGame->MeleeEnemyLeftAmmo[i];
				SpawnedMeleeEnemyController->RightAmmo = LoadedGame->MeleeEnemyRightAmmo[i];
				if (SpawnedMeleeEnemyController->LeftAmmo <= 0)
				{
					SpawnedMeleeEnemy->LeftWeapon->Destroy();
				} else
				{
					if (ABomb* Bomb = Cast<ABomb>(SpawnedMeleeEnemy->LeftWeapon))
					{
						Bomb->BombMesh->SetWorldLocation(LoadedGame->MeleeEnemyLeftWeaponLocations[i]);
						Bomb->BombMesh->SetAllPhysicsLinearVelocity(LoadedGame->MeleeEnemyLeftWeaponVelocities[i]);
					}
				}
				if (SpawnedMeleeEnemyController->RightAmmo <= 0)
				{
					SpawnedMeleeEnemy->RightWeapon->Destroy();
				}
			}
		}
	}

	for (int i = 0; i < LoadedGame->BulletLocations.Num(); i++)
	{
		ABullet* SpawnedBullet = Cast<ABullet>(GetWorld()->SpawnActor(BulletClass));
		if (SpawnedBullet)
		{
			SpawnedBullet->SetActorLocation(LoadedGame->BulletLocations[i]);
			SpawnedBullet->SetActorRotation(LoadedGame->BulletRotations[i]);
		}
	}
}

void ABeatEmUpGameMode::Save(UBeatEmUpSaveGame* SaveGame)
{
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

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Enemies);
	for (int i = 0; i < Enemies.Num(); i++)
	{
		AMeleeEnemy* CurrentMeleeEnemy = Cast<AMeleeEnemy>(Enemies[i]);
		if (!CurrentMeleeEnemy)
		{
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
		} else
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
		}
	}

	TArray<AActor*> Bullets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABullet::StaticClass(), Bullets);
	for (int i = 0; i < Bullets.Num(); i++)
	{
		ABullet* CurrentBullet = Cast<ABullet>(Bullets[i]);
		SaveGame->BulletLocations.Add(CurrentBullet->GetActorLocation());
		SaveGame->BulletRotations.Add(CurrentBullet->GetActorRotation());
	}

	TArray<AActor*> DynamicColorFloors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADynamicColorFloor::StaticClass(), DynamicColorFloors);
	for (int i = 0; i < DynamicColorFloors.Num(); i++)
	{
		ADynamicColorFloor* CurrentFloor = Cast<ADynamicColorFloor>(DynamicColorFloors[i]);
		//FLinearColor RetrievedColor;
		//SaveGame->DynamicFloorColors.Add(CurrentFloor->EmissiveMaterialInstance->GetVectorParameterValue(FName("BaseColor"), RetrievedColor));
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
