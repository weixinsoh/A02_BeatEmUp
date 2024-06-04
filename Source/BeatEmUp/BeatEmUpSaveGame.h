// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/SaveGame.h"
#include "BeatEmUpSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API UBeatEmUpSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		FString SaveSlotName;

	UPROPERTY()
		uint32 UserIndex;

	UPROPERTY()
		bool bBossLevelLoaded;
	
	UPROPERTY()
		FVector PlayerPosition;
	
	UPROPERTY()
		FRotator PlayerRotation;
	
	UPROPERTY()
		float PlayerCurrentHealth;
	
	UPROPERTY()
		float PlayerMaxHealth;
	
	UPROPERTY()
		float PlayerCurrentEXP;
	
	UPROPERTY()
		float PlayerEXPToLevel;
	
	UPROPERTY()
		float PlayerPunchDamage;

	UPROPERTY()
		TArray<AWeapon*> PlayerInventory;

	UPROPERTY()
		AWeapon* PlayerEquippingWeapon;

	UPROPERTY()
		float PlayerNumOfEnemiesDefeated;

	UPROPERTY()
		TArray<FVector> EnemyLocations;
	
	UPROPERTY()
		TArray<FRotator> EnemyRotations;

	UPROPERTY()
		TArray<float> EnemyCurrentHealths;

	UPROPERTY()
		TArray<float> EnemyMaxHealths;

	UPROPERTY()
		TArray<bool> EnemyRagdollStates;

	UPROPERTY()
		TArray<FVector> EnemyMeshLocations;

	UPROPERTY()
		TArray<FVector> EnemyMeshVelocities;

	UPROPERTY()
		TArray<float> EnemyAmmo;

	UPROPERTY()
		TArray<bool> EnemyIsBossChild;

	UPROPERTY()
		TArray<FVector> MeleeEnemyLocations;
	
	UPROPERTY()
		TArray<FRotator> MeleeEnemyRotations;

	UPROPERTY()
		TArray<float> MeleeEnemyCurrentHealths;

	UPROPERTY()
		TArray<float> MeleeEnemyMaxHealths;

	UPROPERTY()
		TArray<bool> MeleeEnemyRagdollStates;

	UPROPERTY()
		TArray<FVector> MeleeEnemyMeshLocations;

	UPROPERTY()
		TArray<FVector> MeleeEnemyMeshVelocities;

	UPROPERTY()
		TArray<float> MeleeEnemyMaxWalkspeed;

	UPROPERTY()
		TArray<float> MeleeEnemyLeftAmmo;

	UPROPERTY()
		TArray<float> MeleeEnemyRightAmmo;

	UPROPERTY()
		TArray<FVector> MeleeEnemyLeftWeaponLocations;

	UPROPERTY()
		TArray<FVector> MeleeEnemyLeftWeaponVelocities;

	UPROPERTY()
		TArray<FVector> BulletLocations;

	UPROPERTY()
		TArray<FRotator> BulletRotations;

	UPROPERTY()
		TArray<FVector> BombLocations;

	UPROPERTY()
		TArray<FVector> BombVelocities;

	UPROPERTY()
		TArray<FLinearColor> DynamicFloorColors;

	UPROPERTY()
		TArray<FVector> StickLocations;

	UPROPERTY()
		TArray<FRotator> StickRotations;

	UPROPERTY()
		TArray<bool> DestroyedSticks;

	UPROPERTY()
		TArray<FVector> StickVelocities;

	UPROPERTY()
		TArray<bool> ActiveSticks;

	UPROPERTY()
		TArray<float> StickOpacities;

	UPROPERTY()
		TArray<bool> ToBeProjectSticks;

	UPROPERTY()
		TArray<float> StickProjectTimes;
	
	UPROPERTY()
		FVector BossEnemyLocation;
	
	UPROPERTY()
		FRotator BossEnemyRotation;

	UPROPERTY()
		float BossEnemyCurrentHealth;

	UPROPERTY()
		float BossEnemyMaxHealth;

	UPROPERTY()
		bool BossEnemyRagdollState;

	UPROPERTY()
		FVector BossEnemyMeshLocation;

	UPROPERTY()
		FVector BossEnemyMeshVelocity;

	UPROPERTY()
		bool bIsLeftChildDefeated;

	UPROPERTY()
		bool bIsRightChildDefeated;

};
