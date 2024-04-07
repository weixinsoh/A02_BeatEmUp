// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class BEATEMUP_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere)
		float SpawnFrequency = 1;

	UPROPERTY(EditAnywhere)
		int MaxEnemies = 10;

	UPROPERTY(EditAnywhere)
		float SpawnRadius = 1000;

	FTimerHandle SpawnTimerHandle;

	UFUNCTION()
		void SpawnEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
