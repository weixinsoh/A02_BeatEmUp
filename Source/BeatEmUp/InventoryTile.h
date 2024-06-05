// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryDescriptionUI.h"
#include "Weapon.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "InventoryTile.generated.h"

/**
 * 
 */

UCLASS()
class BEATEMUP_API UInventoryTile : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		UImage* WeaponIcon;

	UPROPERTY(meta = (BindWidget))
		UButton* TileButton;

	UPROPERTY(meta = (BindWidget))
		UBorder* TileBorder;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UInventoryDescriptionUI> InventoryDescriptionClass;

	UPROPERTY(EditAnywhere)
		UInventoryDescriptionUI* InventoryDescriptionUI;

	FTimerHandle ShowTimeHandle;

	float ShowTime = 2;
	
	void SetTile(UTexture2D* Icon, AWeapon* Weapon);

	UFUNCTION()
		void OnHovered();

	UFUNCTION()
		void OnUnHovered();
	
	bool bIsHovered;
};
