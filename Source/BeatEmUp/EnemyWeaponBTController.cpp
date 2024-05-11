// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponBTController.h"

#include "MeleeEnemy.h"
#include "Perception/AIPerceptionComponent.h"
#include "Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"

/**
 * Constructor which initializes an instance of AEnemyWeaponBTController
 * and configures its perception system to enable AI detection of various entities.
 */
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

/**
 * Begin play function which allows initialization of behaviours for the actor.
 */
void AEnemyWeaponBTController::BeginPlay()
{
	Super::BeginPlay();

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviourTree);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyWeaponBTController::OnSensesUpdated);

	// Set initial blackboard values related to ammo status.
	BlackboardComponent->SetValueAsBool("HasAmmo", true);
	BlackboardComponent->SetValueAsBool("HasLeftAmmo", true);
	BlackboardComponent->SetValueAsBool("HasRightAmmo", true);

	// Store enemy movement speed
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
	WalkingSpeed = MeleeEnemy->GetCharacterMovement()->MaxWalkSpeed;
	SprintSpeed = WalkingSpeed + 500;
}

/**
 * This function updates the blackboard value "PlayerPosition"
 * as the target player current location.
 */
void AEnemyWeaponBTController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

/**
 * Returns the control rotation of the controller.
 * @return The control rotation of the controller.
 */
FRotator AEnemyWeaponBTController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return Super::GetControlRotation();
}

/**
 * This function is used to generate random location to patrol.
 */
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

/**
 * This function is used to update the blackboard values once the senses updated.
 * @param UpdatedActor The actor that the enemy AI has sensed.
 * @param Stimulus The stimulus that triggered the perception update.
 */
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
				BlackboardComponent->SetValueAsBool("NeedsCheck", true);
			}
			else
			{
				TargetPlayer = nullptr;
				BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			}
		}
	}
}

/**
 * This function is used to check whether the target player is within the attackable range.
 * It determines which weapon to be used to attack the target player.
 */
void AEnemyWeaponBTController::RangeCheck()
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
	float AttackableDistance;
	float Distance = FVector::Distance(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation());

	// Use right weapon if left weapon has no ammo
	if (LeftAmmo <= 0)
	{
		AttackableDistance = MeleeEnemy->RightWeapon->AttackDistance;
		BlackboardComponent->SetValueAsBool("UseRightWeapon", Distance <= AttackableDistance);
	}
	// Use left weapon if right weapon has no ammo
	else if (RightAmmo <= 0)
	{
		AttackableDistance = MeleeEnemy->LeftWeapon->AttackDistance;
		BlackboardComponent->SetValueAsBool("UseLeftWeapon", Distance <= AttackableDistance);
	}
	else
	{
		AttackableDistance = FMath::Max(MeleeEnemy->LeftWeapon->AttackDistance,
		                                MeleeEnemy->RightWeapon->AttackDistance);
		
		// Use the long range weapon if both weapon are available to attack
		if (Distance <= MeleeEnemy->LeftWeapon->AttackDistance && Distance <= MeleeEnemy->RightWeapon->AttackDistance)
		{
			if (MeleeEnemy->LeftWeapon->AttackDistance <= MeleeEnemy->RightWeapon->AttackDistance)
			{
				BlackboardComponent->SetValueAsBool("UseLeftWeapon", true);
			}
			else
			{
				BlackboardComponent->SetValueAsBool("UseRightWeapon", true);
			}
		}
		// Use weapon that is within attackable range
		else
		{
			if (Distance <= MeleeEnemy->LeftWeapon->AttackDistance)
			{
				BlackboardComponent->SetValueAsBool("UseLeftWeapon", true);
			}
			else if (Distance <= MeleeEnemy->RightWeapon->AttackDistance)
			{
				BlackboardComponent->SetValueAsBool("UseRightWeapon", true);
			}
		}
	}
	BlackboardComponent->SetValueAsBool("IsNotWithinAttackableRange", Distance > AttackableDistance);
	BlackboardComponent->SetValueAsBool("NeedsCheck", false);
}

/**
 * This function is used to calculate the new position to move nearer
 * to the target player so that it is within the attackable range.
 */
void AEnemyWeaponBTController::SprintToAttackableRange()
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
	if (MeleeEnemy)
	{
		// Set the enemy AI to sprint
		SetMovementSpeed(true);

		// Calculate the new position the enemy AI required to move nearer to the target player
		if (TargetPlayer)
		{
			FVector Direction = TargetPlayer->GetActorLocation() - MeleeEnemy->GetActorLocation();
			Direction.Normalize();
			float AttackableDistance;

			if (LeftAmmo <= 0)
			{
				AttackableDistance = MeleeEnemy->RightWeapon->AttackDistance;
			}
			else if (RightAmmo <= 0)
			{
				AttackableDistance = MeleeEnemy->RightWeapon->AttackDistance;
			}
			else
			{
				AttackableDistance = FMath::Max(MeleeEnemy->LeftWeapon->AttackDistance,
				                                MeleeEnemy->RightWeapon->AttackDistance);
			}
			
			float Distance = FVector::Distance(TargetPlayer->GetActorLocation(), GetPawn()->GetActorLocation()) - AttackableDistance;
			FVector Position = GetPawn()->GetActorLocation() + Direction * Distance;
			BlackboardComponent->SetValueAsVector("SprintToPoint", Position);
			BlackboardComponent->SetValueAsBool("NeedsCheck", true);
		}
	}
}

/**
 * This function is used to set the movement speed of the enemy AI.
 * @param bSprint A boolean representing the enemy AI is sprinting
 */
void AEnemyWeaponBTController::SetMovementSpeed(bool bSprint)
{
	AMeleeEnemy* MeleeEnemy = Cast<AMeleeEnemy>(GetPawn());
	if (bSprint)
	{
		MeleeEnemy->SetMovementSpeed(SprintSpeed);
	}
	else
	{
		MeleeEnemy->SetMovementSpeed(WalkingSpeed);
	}
}

/**
 * This function is used to perform the attack action using the left weapon.
 */
void AEnemyWeaponBTController::UseLeftWeapon()
{
	Cast<AMeleeEnemy>(GetPawn())->UseLeftWeapon();
	LeftAmmo--;
	BlackboardComponent->SetValueAsBool("HasLeftAmmo", LeftAmmo > 0);
	BlackboardComponent->SetValueAsBool("HasAmmo", LeftAmmo > 0 || RightAmmo > 0);
	BlackboardComponent->SetValueAsBool("UseLeftWeapon", false);
	BlackboardComponent->SetValueAsBool("NeedsCheck", true);
}

/**
 * This function is used to perform the attack action using the right weapon.
 */
void AEnemyWeaponBTController::UseRightWeapon()
{
	Cast<AMeleeEnemy>(GetPawn())->UseRightWeapon();
	RightAmmo--;
	BlackboardComponent->SetValueAsBool("HasRightAmmo", RightAmmo > 0);
	BlackboardComponent->SetValueAsBool("HasAmmo", LeftAmmo > 0 || RightAmmo > 0);
	BlackboardComponent->SetValueAsBool("UseRightWeapon", false);
	BlackboardComponent->SetValueAsBool("NeedsCheck", true);
}
