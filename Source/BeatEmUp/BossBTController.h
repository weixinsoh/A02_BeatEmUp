// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NiagaraSystem.h"
#include "BossBTController.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API ABossBTController : public AAIController
{
	GENERATED_BODY()

public:
	ABossBTController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	/*
	void GenerateNewRandomLocation();

	UFUNCTION()
	void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere)
	float SightRadius = 2000;

	UPROPERTY(EditAnywhere)
	float SightAge = 3.5;

	UPROPERTY(EditAnywhere)
	float LoseSightRadius = SightRadius + 100;

	UPROPERTY(EditAnywhere)
	float FieldOfView = 45;

	UPROPERTY(EditAnywhere)
	float PatrolDistance = 2000;

	UPROPERTY(EditAnywhere)
	UAISenseConfig_Sight* SightConfiguration;
*/
	UPROPERTY(EditAnywhere)
	UBlackboardData* AIBlackboard;

	UPROPERTY(EditAnywhere)
	UBehaviorTree* BehaviourTree;

	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY()
	UNavigationSystemV1* NavigationSystem;

	UPROPERTY()
	APawn* TargetPlayer;

	UFUNCTION()
	void Teleport();

	UFUNCTION()
	void TeleportEnd();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void SpawnEnemy();

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* TeleportTrailSystem;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* TeleportTrailComponent;
	
};
