// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "BeatEmUpCharacter.h"
#include "Flail.h"
#include "InventoryTile.h"

/**
 * This function is used to changed the equipping weapon
 * to the clicked weapon in the inventory for the player. 
 * @param Index the index of the weapon located in the inventory.
 */
void UInventoryWidget::OnButtonWasClicked(int32 Index)
{
	if (Index - 1 >= 0)
	{
		TileList[Index - 1]->TileBorder->SetBrushColor(FLinearColor::White);
	} else
	{
		TileList[TileList.Num() - 1]->TileBorder->SetBrushColor(FLinearColor::White);
	}
	TileList[Index]->TileBorder->SetBrushColor(FLinearColor::Red);
	if (Owner->EquippingWeapon)
	{
		Owner->EquippingWeapon->SetActorHiddenInGame(true);
		Owner->EquippingWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	AWeapon* SelectedWeapon = Owner->GetWeaponAtIndex(Index);
	if(!SelectedWeapon)
	{
		return;
	}
	Owner->EquippingWeapon = SelectedWeapon;
	Owner->EquippingWeapon->SetActorHiddenInGame(false);
	if (Cast<AFlail>(Owner->EquippingWeapon))
	{
		return;
	}
	FName WeaponSocket = FName("WeaponSocket");
	Owner->EquippingWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
}

/**
 * This function is used to update the inventory widget if any changes in the inventory is detected. 
 * @param Weapons An array of weapons in the inventory.
 */
void UInventoryWidget::RefreshInventory(TArray<AWeapon*> Weapons)
{
	for (UInventoryTile* Tile : TileList)
	{
		Tile->WeaponIcon->SetBrushFromTexture(*IconMap.Find("None"));
		Tile->InventoryDescriptionUI = nullptr;
	}

	for (int I = 0; I < Weapons.Num(); I++)
	{
		// check weapons list and tile list
		TileList[I]->SetTile(*IconMap.Find(Weapons[I]->GetName()), Weapons[I]);
	}
}

/**
 * This function creates the inventory tiles widget to build the inventory grid
 * and dynamically delegate OnButtonWasClicked() function once the tile is clicked.
 */
void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TileList.Num() > 0)
	{
		return;
	}

	for(int32 Y = 0; Y < 4; Y++)
	{
		UInventoryTile* Tile = Cast<UInventoryTile>(CreateWidget(GetGameInstance(), BPInventoryTile));

		TileList.Add(Tile);
		InventoryGrid->AddChildToGrid(Tile, 0, Y);
	}

}
