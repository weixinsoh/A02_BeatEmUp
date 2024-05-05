// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryTile.h"
#include "Blueprint/UserWidget.h"
#include "Components/GridPanel.h"
#include "InventoryWidget.generated.h"

class ABeatEmUpCharacter;

/**
 * 
 */

UCLASS()
class BEATEMUP_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UInventoryTile> BPInventoryTile;

	UPROPERTY()
	TArray<UInventoryTile*> TileList;
	
	UPROPERTY(meta=(BindWidget))
	UGridPanel* InventoryGrid;

	UFUNCTION()
	void OnButtonWasClicked(UInventoryTile* Tile);

	void RefreshInventory(TArray<AWeapon*> Weapons);

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	ABeatEmUpCharacter* Owner;

	UPROPERTY(EditAnywhere)
	TMap<FString, UTexture2D*> IconMap;
};