// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeCheckTask.h"

#include "EnemyBTController.h"
#include "EnemyWeaponBTController.h"

/**
 * This function is used to execute the task to check the target player is within the attackable range. 
 */
EBTNodeResult::Type URangeCheckTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if(WeaponBTController)
	{
		WeaponBTController->RangeCheck();

	}
	return EBTNodeResult::Succeeded;}
