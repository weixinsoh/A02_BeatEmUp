// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTile.h"

void UInventoryTile::SetTile(UTexture2D* Icon)
{
	WeaponIcon->SetBrushFromTexture(Icon);
}

void UInventoryTile::NativeConstruct()
{
	Super::NativeConstruct();

	TileButton->OnClicked.AddUniqueDynamic(this, &UInventoryTile::OnClicked);
}

void UInventoryTile::OnClicked()
{
	OnClickedDelegate.Broadcast(this);
}


