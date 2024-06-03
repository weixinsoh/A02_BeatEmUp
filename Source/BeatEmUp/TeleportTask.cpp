// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportTask.h"

#include "BossBTController.h"

EBTNodeResult::Type UTeleportTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	BTController->Teleport();
	return EBTNodeResult::Succeeded;
}
