// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeatEmUpCharacter.h"
#include "UObject/NoExportTypes.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API UWeapon : public UObject
{
	GENERATED_BODY()

public:
	UWeapon();

	FString WeaponName;

	FString WeaponDescription;

	void SetName(FString Name);
	void SetDescription(FString Description);

	FString GetName();
	FString GetDescription();

	virtual void UseWeapon(ACharacter* Character);
};
