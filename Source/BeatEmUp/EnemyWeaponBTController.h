// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NavigationSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "Weapon.h"
#include "EnemyWeaponBTController.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AEnemyWeaponBTController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyWeaponBTController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void GenerateNewRandomLocation();

	UFUNCTION()
		void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere)
		float SightRadius = 1000;

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

	UPROPERTY(EditAnywhere)
		float LeftAmmo = 5;

	UPROPERTY(EditAnywhere)
		float RightAmmo = 5;

	UPROPERTY(EditAnywhere)
		float WalkingSpeed;

	UPROPERTY(EditAnywhere)
		float SprintSpeed;

	void RangeCheck();
	void SprintToAttackableRange();
	void SetMovementSpeed(bool bSprint);
	void UseLeftWeapon();
	void UseRightWeapon();
};


