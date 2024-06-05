// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBTController.h"

#include "BossEnemy.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

/**
 * Constructor which initializes an instance of ABossBTController
 */
ABossBTController::ABossBTController()
{
	PrimaryActorTick.bCanEverTick = true;
}

/**
 * Begin play function which allows initialization of behaviours for the actor.
 */
void ABossBTController::BeginPlay()
{
	Super::BeginPlay();
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviourTree);
	TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	BlackboardComponent->SetValueAsBool("SpawnEnemy", true);
}

/**
 * This function updates the blackboard value "PlayerPosition"
 * as the target player current location and keeps track of the children current health.
 */
void ABossBTController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Updates player position
	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}

	// Keeps track left child's current health
	if (AEnemy* LeftChild = Cast<AEnemy>(LeftEnemyChild))
	{
		if (LeftChild->CurrentHealth <= 0)
		{
			bIsLeftChildDefeated = true;
		}
	}

	// Keeps track right child's current health
	if (AEnemy* RightChild = Cast<AEnemy>(RightEnemyChild))
	{
		if (RightChild->CurrentHealth <= 0)
		{
			bIsRightChildDefeated = true;
		}
	}

	// Spawn enemy if both children are defeated
	if (bIsLeftChildDefeated && bIsRightChildDefeated)
	{
		BlackboardComponent->SetValueAsBool("SpawnEnemy", true);
	}
}

/**
 * Returns the control rotation of the controller.
 * @return The control rotation of the controller.
 */
FRotator ABossBTController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return Super::GetControlRotation();
}


/**
 * This function is used to teleport around the target.
 */
void ABossBTController::Teleport()
{
	if (GetPawn())
	{
		// Destroy the particle effect after an attack
		if (TeleportTrailComponent)
		{
			TeleportTrailComponent->DestroyComponent();
		}

		// Calculate the teleport position
		FVector RandomLocation = FMath::VRand() * 500;
		RandomLocation.Z = GetPawn()->GetActorLocation().Z;
		FVector TeleportPosition = TargetPlayer->GetActorLocation() + RandomLocation;
		TeleportPosition.Z = GetPawn()->GetActorLocation().Z;
		BlackboardComponent->SetValueAsVector("TeleportPosition", TeleportPosition);

		// Teleport the boss enemy
		ABossEnemy* BossEnemy = Cast<ABossEnemy>(GetPawn());
		BossEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		BossEnemy->GetCharacterMovement()->MaxFlySpeed = 5000;
		BossEnemy->GetCharacterMovement()->MaxAcceleration = 999999;
		BossEnemy->GetMesh()->SetVisibility(false, true);
		BossEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

/**
 * This function is used to set the state of the boss enemy to the previous state after teleporting.
 */
void ABossBTController::TeleportEnd()
{
	if (GetPawn())
	{
		ABossEnemy* BossEnemy = Cast<ABossEnemy>(GetPawn());
		BossEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		BossEnemy->GetCharacterMovement()->MaxAcceleration = 2048;
		BossEnemy->GetMesh()->SetVisibility(true, true);
		BossEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	}
}

/**
 * This function is used to generate thr particle effect and attack the target. 
 */
void ABossBTController::Attack()
{
	if (GetPawn())
	{
		ABossEnemy* BossEnemy = Cast<ABossEnemy>(GetPawn());
		TeleportTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TeleportTrailSystem, BossEnemy->GetMesh(), FName("spine_01"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		BossEnemy->Attack();
	}
}

/**
 * This function is used to spawn the child enemies.
 */
void ABossBTController::SpawnEnemy()
{
	if (GetPawn())
	{
		ABossEnemy* BossEnemy = Cast<ABossEnemy>(GetPawn());
		BossEnemy->SpawnEnemy();
		BlackboardComponent->SetValueAsBool("SpawnEnemy", false);
	}
}


