// Fill out your copyright notice in the Description page of Project Settings.


#include "UseLeftWeaponTask.h"

#include "EnemyWeaponBTController.h"

/**
 * This function is used to execute the attack task using left weapon. 
 */
EBTNodeResult::Type UUseLeftWeaponTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	if (!BTComp)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyWeaponBTController* BTController = Cast<AEnemyWeaponBTController>(BTComp->GetOwner());
	if (!BTController)
	{
		return EBTNodeResult::Failed;
	}

	BTController->UseLeftWeapon();
	return EBTNodeResult::Succeeded;
}
