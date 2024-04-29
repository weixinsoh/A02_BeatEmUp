// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NavigationSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBTController.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AEnemyBTController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyBTController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void GenerateNewRandomLocation();

	UFUNCTION()
		void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere)
		float SightRadius = 500;
	UPROPERTY(EditAnywhere)
		float SightAge = 3.5;
	UPROPERTY(EditAnywhere)
		float LoseSightRadius = SightRadius + 50;
	UPROPERTY(EditAnywhere)
		float FieldOfView = 45;
	UPROPERTY(EditAnywhere)
		float PatrolDistance = 2000;
	UPROPERTY(EditAnywhere)
		UAISenseConfig_Sight* SightConfiguration;
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
	
};
