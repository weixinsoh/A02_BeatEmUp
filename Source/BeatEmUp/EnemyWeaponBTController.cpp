// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWeaponBTController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Weapon.h"


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

	BlackboardComponent->SetValueAsBool("HasWeapon", true);
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
	AWeapon* SensedWeapon = Cast<AWeapon>(UpdatedActor);
	if (SensedWeapon)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			TargetWeapon = SensedWeapon;
			BlackboardComponent->SetValueAsBool("WalkToWeapon", true);
			BlackboardComponent->SetValueAsVector("WeaponPosition", TargetWeapon->GetActorLocation());
		}
		else
		{
			TargetWeapon = nullptr;
			BlackboardComponent->SetValueAsBool("WalkToWeapon", false);
		}
	}
	
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
			}
			else
			{
				TargetPlayer = nullptr;
				BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			}
		}
	}
}

void AEnemyWeaponBTController::UseWeapon()
{
	
}
