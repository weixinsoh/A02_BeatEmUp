// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackTask.h"

#include "BossBTController.h"

/**
 * This function is used to execute the attack task. 
 */
EBTNodeResult::Type UAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	BTController->Attack();
	return EBTNodeResult::Succeeded;}
