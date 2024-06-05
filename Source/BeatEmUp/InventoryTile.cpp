// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryTile.h"

#include "InventoryDescriptionUI.h"
#include "Kismet/GameplayStatics.h"

void UInventoryTile::SetTile(UTexture2D* Icon, AWeapon* Weapon)
{
	// Set tile display icon
	WeaponIcon->SetBrushFromTexture(Icon);

	// Create widget for weapon info
	InventoryDescriptionUI = Cast<UInventoryDescriptionUI>(CreateWidget(GetGameInstance(), InventoryDescriptionClass));
	InventoryDescriptionUI->NameText->SetText(FText::FromString(FString(Weapon->WeaponName)));
	InventoryDescriptionUI->AttackDistanceText->SetText(FText::FromString(FString::SanitizeFloat(Weapon->AttackDistance)));
	InventoryDescriptionUI->AttackSpeedText->SetText(FText::FromString(FString::SanitizeFloat(Weapon->AttackSpeed)));
	InventoryDescriptionUI->DamageText->SetText(FText::FromString(FString::SanitizeFloat(Weapon->Damage)));
	InventoryDescriptionUI->DescriptionText->SetText(FText::FromString(FString(Weapon->WeaponDescription)));
	InventoryDescriptionUI->SetVisibility(ESlateVisibility::Hidden);
	InventoryDescriptionUI->AddToViewport();
}

/**
 * This function set the weapon information widget to be visible
 * when the mouse wheel is focusing the tile.
 */
void UInventoryTile::OnHovered()
{
	if (InventoryDescriptionUI)
	{
		bIsHovered = true;
		InventoryDescriptionUI->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().SetTimer(ShowTimeHandle, this, &UInventoryTile::OnUnHovered, ShowTime, false); 
	}
}

/**
 * This function set the weapon information widget to be hidden
 * when the mouse wheel no longer focusing the tile.
 */
void UInventoryTile::OnUnHovered()
{
	if (InventoryDescriptionUI)
	{
		bIsHovered = false;
		InventoryDescriptionUI->SetVisibility(ESlateVisibility::Hidden);
	}
}



