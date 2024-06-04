// Fill out your copyright notice in the Description page of Project Settings.


#include "BossBTController.h"

#include "BossEnemy.h"
#include "NiagaraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


ABossBTController::ABossBTController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABossBTController::BeginPlay()
{
	Super::BeginPlay();
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviourTree);
	TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	BlackboardComponent->SetValueAsBool("SpawnEnemy", true);
}

void ABossBTController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
	
	if (AEnemy* LeftChild = Cast<AEnemy>(LeftEnemyChild))
	{
		if (LeftChild->CurrentHealth <= 0)
		{
			bIsLeftChildDefeated = true;
		}
	}
	if (AEnemy* RightChild = Cast<AEnemy>(RightEnemyChild))
	{
		if (RightChild->CurrentHealth <= 0)
		{
			bIsRightChildDefeated = true;
		}
	}
	if (bIsLeftChildDefeated && bIsRightChildDefeated)
	{
		BlackboardComponent->SetValueAsBool("SpawnEnemy", true);
	}
}

FRotator ABossBTController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return Super::GetControlRotation();
}

void ABossBTController::Teleport()
{
	if (GetPawn())
	{
		if (TeleportTrailComponent)
		{
			TeleportTrailComponent->DestroyComponent();
		}
		FVector RandomLocation = FMath::VRand() * 500;
		RandomLocation.Z = GetPawn()->GetActorLocation().Z;
		FVector TeleportPosition = TargetPlayer->GetActorLocation() + RandomLocation;
		TeleportPosition.Z = GetPawn()->GetActorLocation().Z;
		BlackboardComponent->SetValueAsVector("TeleportPosition", TeleportPosition);
		
		ABossEnemy* BossEnemy = Cast<ABossEnemy>(GetPawn());
		BossEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		BossEnemy->GetCharacterMovement()->MaxFlySpeed = 5000;
		BossEnemy->GetCharacterMovement()->MaxAcceleration = 999999;
		BossEnemy->GetMesh()->SetVisibility(false, true);
		BossEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	}
}

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

void ABossBTController::Attack()
{
	if (GetPawn())
	{
		ABossEnemy* BossEnemy = Cast<ABossEnemy>(GetPawn());
		TeleportTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(TeleportTrailSystem, BossEnemy->GetMesh(), FName("spine_01"), FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, true);
		BossEnemy->Attack();
	}
}

void ABossBTController::SpawnEnemy()
{
	if (GetPawn())
	{
		ABossEnemy* BossEnemy = Cast<ABossEnemy>(GetPawn());
		BossEnemy->SpawnEnemy();
		BlackboardComponent->SetValueAsBool("SpawnEnemy", false);
	}
}


