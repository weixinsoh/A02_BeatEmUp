// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportEndTask.h"
#include "BossBTController.h"

/**
 * This function is used to execute the task to set the boss back to the previous state after teleporting.
 */
EBTNodeResult::Type UTeleportEndTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	if (!BTComp)
	{
		return EBTNodeResult::Failed;
	}

	ABossBTController* BTController = Cast<ABossBTController>(BTComp->GetOwner());
	if (!BTController)
	{
		return EBTNodeResult::Failed;
	}

	BTController->TeleportEnd();
	return EBTNodeResult::Succeeded;
}