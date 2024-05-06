// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponBTController.h"

#include "MeleeEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyWeaponBTController::AEnemyWeaponBTController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
}

void AEnemyWeaponBTController::BeginPlay()
{
	Super::BeginPlay();

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviourTree);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyWeaponBTController::OnSensesUpdated);
	BlackboardComponent->SetValueAsBool("HasAmmo", true);
	BlackboardComponent->SetValueAsBool("HasLeftAmmo", true);
	BlackboardComponent->SetValueAsBool("HasRightAmmo", true);
}

void AEnemyWeaponBTController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

FRotator AEnemyWeaponBTController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return Super::GetControlRotation();
}

void AEnemyWeaponBTController::GenerateNewRandomLocation()
{
	if (NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->
			GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), PatrolDistance, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

void AEnemyWeaponBTController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	APawn* SensedPawn = Cast<APawn>(UpdatedActor);
	if (SensedPawn)
	{
		if (SensedPawn->IsPlayerControlled())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				TargetPlayer = SensedPawn;
				BlackboardComponent->SetValueAsBool("ChasePlayer", true);
				BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
				
				AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
				float Distance = FVector::Distance(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation());
				if (Distance <= MeleeEnemy->LeftWeapon->AttackDistance)
				{
					BlackboardComponent->SetValueAsBool("UseLeftWeapon", true);
				} else if (Distance <= MeleeEnemy->RightWeapon->AttackDistance)
				{
					BlackboardComponent->SetValueAsBool("UseRightWeapon", true);
				}
			}
			else
			{
				TargetPlayer = nullptr;
				BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			}
		}
	}
}

void AEnemyWeaponBTController::UseLeftWeapon()
{
	Cast<AMeleeEnemy>(GetPawn())->UseLeftWeapon();
	LeftAmmo--;
	BlackboardComponent->SetValueAsBool("HasLeftAmmo", LeftAmmo > 0);
	BlackboardComponent->SetValueAsBool("HasAmmo", LeftAmmo > 0 || RightAmmo > 0);
	BlackboardComponent->SetValueAsBool("UseLeftWeapon", false);
}

void AEnemyWeaponBTController::UseRightWeapon()
{
	Cast<AMeleeEnemy>(GetPawn())->UseLeftWeapon();
	RightAmmo--;
	BlackboardComponent->SetValueAsBool("HasRightAmmo", RightAmmo > 0);
	BlackboardComponent->SetValueAsBool("HasAmmo", LeftAmmo > 0 || RightAmmo > 0);
	BlackboardComponent->SetValueAsBool("UseRightWeapon", false);
}

void AEnemyWeaponBTController::SprintToAttackableRange()
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
	WalkingSpeed = MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed;
	SetMovementSpeed(SprintSpeed);
	FVector Direction = MeleeEnemy->GetActorLocation() - TargetPlayer->GetActorLocation();
	Direction.Normalize();
	float AttackableDistance;
	
	if (LeftAmmo < 0)
	{
		AttackableDistance = MeleeEnemy->RightWeapon->AttackDistance;
	} else if (RightAmmo < 0)
	{
		AttackableDistance = MeleeEnemy->RightWeapon->AttackDistance;
	} else
	{
		AttackableDistance = FMath::Min(MeleeEnemy->LeftWeapon->AttackDistance, MeleeEnemy->RightWeapon->AttackDistance);
	}
	float Distance = FVector::Distance(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation()) - AttackableDistance;
	FVector Position = Direction * Distance;
	BlackboardComponent->SetValueAsVector("SprintToPoint", Position);
}

void AEnemyWeaponBTController::SetMovementSpeed(float Speed)
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
	MeleeEnemy->SetMovementSpeed(Speed);
}


void AEnemyWeaponBTController::RangeCheck()
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
	float AttackableDistance;
	
	if (LeftAmmo < 0)
	{
		AttackableDistance = MeleeEnemy->RightWeapon->AttackDistance;
	} else if (RightAmmo < 0)
	{
		AttackableDistance = MeleeEnemy->RightWeapon->AttackDistance;
	} else
	{
		AttackableDistance = FMath::Min(MeleeEnemy->LeftWeapon->AttackDistance, MeleeEnemy->RightWeapon->AttackDistance);
	}
	float Distance = FVector::Distance(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation());
	BlackboardComponent->SetValueAsBool("IsWithinAttackableRange", Distance <= AttackableDistance);
}



