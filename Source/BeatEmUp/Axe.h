// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Axe.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AAxe : public AWeapon
{
	GENERATED_BODY()

public:

	AAxe();
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* AxeHead;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* AxeHandler;

	UPROPERTY(EditAnywhere)
		float HitForce = 50000;

	virtual void BeginPlay() override;
	virtual void UseWeapon(ACharacter* Character) override;

};
