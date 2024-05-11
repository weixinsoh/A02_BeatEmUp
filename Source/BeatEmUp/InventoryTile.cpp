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
 * This function dynamically delegate functions
 * once the tile is clicked or hovered/unhovered.
 */
void UInventoryTile::NativeConstruct()
{
	Super::NativeConstruct();

	TileButton->OnClicked.AddUniqueDynamic(this, &UInventoryTile::OnClicked);
	TileButton->OnHovered.AddUniqueDynamic(this,&UInventoryTile::OnHovered);
	TileButton->OnUnhovered.AddUniqueDynamic(this, &UInventoryTile::OnUnHovered);
	
}

/**
 * This function update the position of the weapon information widget
 * to follow the mouse cursor.
 */
void UInventoryTile::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsHovered)
	{
		FVector2D CursorPosition;
		GetOwningPlayer()->GetMousePosition(CursorPosition.X, CursorPosition.Y);
		InventoryDescriptionUI->SetPositionInViewport(CursorPosition);
	}
	
}

/**
 * This function broadcast the OnClicked event,
 * indicating that this object has been clicked.
 */
void UInventoryTile::OnClicked()
{
	OnClickedDelegate.Broadcast(this);
}

/**
 * This function set the weapon information widget to be visible
 * when the mouse is hovering over the tile.
 */
void UInventoryTile::OnHovered()
{
	if (InventoryDescriptionUI)
	{
		bIsHovered = true;
		InventoryDescriptionUI->SetVisibility(ESlateVisibility::Visible);
	}
}

/**
 * This function set the weapon information widget to be hidden
 * when the mouse no longer hovering over the tile.
 */
void UInventoryTile::OnUnHovered()
{
	if (InventoryDescriptionUI)
	{
		bIsHovered = false;
		InventoryDescriptionUI->SetVisibility(ESlateVisibility::Hidden);
	}
}



