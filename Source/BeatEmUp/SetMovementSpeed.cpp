// Fill out your copyright notice in the Description page of Project Settings.


#include "SetMovementSpeed.h"

#include "EnemyWeaponBTController.h"

EBTNodeResult::Type USetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	BTController->SetMovementSpeed(false);
	return EBTNodeResult::Succeeded;}
