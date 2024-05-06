// Fill out your copyright notice in the Description page of Project Settings.


#include "UseRightWeaponTask.h"
#include "EnemyWeaponBTController.h"


EBTNodeResult::Type UUseRightWeaponTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	BTController->UseRightWeapon();
	return EBTNodeResult::Succeeded;
}
