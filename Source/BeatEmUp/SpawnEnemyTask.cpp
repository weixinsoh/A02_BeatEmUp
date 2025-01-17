// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEnemyTask.h"

#include "BossBTController.h"

/**
 * This function is used to execute the task to spawn child enemies. 
 */
EBTNodeResult::Type USpawnEnemyTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	BTController->SpawnEnemy();
	return EBTNodeResult::Succeeded;}
