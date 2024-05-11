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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUInventoryTileOnClicked, class UInventoryTile*, Tile);

UCLASS()
class BEATEMUP_API UInventoryTile : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		UImage* WeaponIcon;

	UPROPERTY(meta = (BindWidget))
		UButton* TileButton;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UInventoryDescriptionUI> InventoryDescriptionClass;

	UPROPERTY(EditAnywhere)
		UInventoryDescriptionUI* InventoryDescriptionUI;
	
	FUInventoryTileOnClicked OnClickedDelegate;

	void SetTile(UTexture2D* Icon, AWeapon* Weapon);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
		void OnClicked();

	UFUNCTION()
		void OnHovered();

	UFUNCTION()
		void OnUnHovered();
	
	bool bIsHovered;
};
