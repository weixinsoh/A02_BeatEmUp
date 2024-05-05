// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	FUInventoryTileOnClicked OnClickedDelegate;

	void SetTile(UTexture2D* Icon);

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClicked();

	int32 Index;
};
