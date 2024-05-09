// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "BeatEmUpCharacter.h"
#include "InventoryTile.h"

void UInventoryWidget::OnButtonWasClicked(UInventoryTile* Tile)
{
	int32 Index = TileList.Find(Tile);
	AWeapon* SelectedWeapon = Owner->GetWeaponAtIndex(Index);
	if(!SelectedWeapon)
	{
		return;
	}
	
	if (Owner->EquippingWeapon)
	{
		Owner->EquippingWeapon->SetActorHiddenInGame(true);
		Owner->EquippingWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	Owner->EquippingWeapon = SelectedWeapon;
	Owner->EquippingWeapon->SetActorHiddenInGame(false);
	FName WeaponSocket = FName("WeaponSocket");
	Owner->EquippingWeapon->AttachToComponent(Owner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
}

void UInventoryWidget::RefreshInventory(TArray<AWeapon*> Weapons)
{
	for (UInventoryTile* Tile : TileList)
	{
		Tile->WeaponIcon->SetBrushFromTexture(*IconMap.Find("None"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Weapons:%d"), Weapons.Num());
	UE_LOG(LogTemp, Warning, TEXT("Tiles:%d"), TileList.Num());

	for (int I = 0; I < Weapons.Num(); I++)
	{
		// check weapons list and tile list
		TileList[I]->SetTile(*IconMap.Find(Weapons[I]->GetName()));
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (TileList.Num() > 0)
	{
		return;
	}

	for(int32 Y = 0; Y < 3; Y++)
	{
		UInventoryTile* Tile = Cast<UInventoryTile>(CreateWidget(GetGameInstance(), BPInventoryTile));

		TileList.Add(Tile);
		Tile->OnClickedDelegate.AddDynamic(this, &UInventoryWidget::OnButtonWasClicked);
		InventoryGrid->AddChildToGrid(Tile, 0, Y);
	}

}
