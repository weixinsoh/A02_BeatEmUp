// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateNewRandomLocationTask.h"

#include "EnemyBTController.h"
#include "EnemyWeaponBTController.h"

EBTNodeResult::Type UGenerateNewRandomLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	if (!BTComp)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyBTController* BTController = Cast<AEnemyBTController>(BTComp->GetOwner());
	AEnemyWeaponBTController* WeaponBTController = Cast<AEnemyWeaponBTController>(BTComp->GetOwner());
	if (!BTController && !WeaponBTController)
	{
		return EBTNodeResult::Failed;
	}

	if(BTController)
	{
		BTController->GenerateNewRandomLocation();

	}
	else if(WeaponBTController)
	{
		WeaponBTController->GenerateNewRandomLocation();

	}
	return EBTNodeResult::Succeeded;
}
