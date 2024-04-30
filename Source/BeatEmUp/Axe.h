// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Axe.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API UAxe : public UWeapon
{
	GENERATED_BODY()

public:
	UAxe();
	virtual void UseWeapon(ACharacter* Character) override;

	UPROPERTY(EditAnywhere)
		float Damage = 150;
	UPROPERTY(EditAnywhere)
		float AttackDistance = 500;
	UPROPERTY(EditAnywhere)
		float AttackSpeed = 50;
	
};
