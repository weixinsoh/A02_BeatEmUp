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

	UPROPERTY()
	AActor* LeftEnemyChild;

	UPROPERTY()
	AActor* RightEnemyChild;

	bool bIsLeftChildDefeated;
	bool bIsRightChildDefeated;

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
