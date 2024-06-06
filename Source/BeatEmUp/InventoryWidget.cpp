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
	// Unhighlight the previous selected weapon
	if (Index - 1 >= 0)
	{
		TileList[Index - 1]->TileBorder->SetBrushColor(FLinearColor::White);
		TileList[Index - 1]->OnUnHovered();
	} else
	{
		TileList[TileList.Num() - 1]->TileBorder->SetBrushColor(FLinearColor::White);
		TileList[TileList.Num() - 1]->OnUnHovered();
	}
	// Highlight the selected weapon
	TileList[Index]->TileBorder->SetBrushColor(FLinearColor::Red);

	// Hide the previous equipping weapon
	if (Owner->EquippingWeapon)
	{
		Owner->EquippingWeapon->SetActorHiddenInGame(true);
		Owner->EquippingWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}

	// Equip the selected weapon
	AWeapon* SelectedWeapon = Owner->GetWeaponAtIndex(Index);
	if(!SelectedWeapon) return;
	Owner->EquippingWeapon = SelectedWeapon;
	Owner->EquippingWeapon->SetActorHiddenInGame(false);
	if (AFlail* Flail = Cast<AFlail>(Owner->EquippingWeapon))
	{
		Flail->Chain10->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LeftWeaponSocket"));
		Flail->BallMesh->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightWeaponSocket"));
	}
	FName WeaponSocket = FName("WeaponSocket");
	Owner->EquippingWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);

	// Show weapon description
	TileList[Index]->OnHovered();
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
