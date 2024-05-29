// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BeatEmUpSaveGame.h"
#include "Enemy.h"
#include "MeleeEnemy.h"
#include "GameFramework/GameModeBase.h"
#include "BeatEmUpGameMode.generated.h"

UCLASS(minimalapi)
class ABeatEmUpGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABeatEmUpGameMode();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AMeleeEnemy> MeleeEnemyClass;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABullet> BulletClass;

	void Load(UBeatEmUpSaveGame* LoadedGame);
	void Save(UBeatEmUpSaveGame* SaveGame);

	UFUNCTION()
		void PostBeginPlay();
};



