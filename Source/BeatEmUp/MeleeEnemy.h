// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyWeaponBTController.h"
#include "CoreMinimal.h"
#include "Axe.h"
#include "Bomb.h"
#include "Enemy.h"
#include "MeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AMeleeEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	AMeleeEnemy();
	virtual void Ragdoll() override;
	virtual void StopRagdoll() override;

	void UseLeftWeapon();
	void UseRightWeapon();

	TSubclassOf<ABomb> BombClass;
	TSubclassOf<AAxe> AxeClass;

	AWeapon* LeftWeapon;
	AWeapon* RightWeapon;
};