// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InventoryDescriptionUI.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API UInventoryDescriptionUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AttackDistanceText;
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* DamageText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* AttackSpeedText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* DescriptionText;

	FVector2D DisplayLocation;
};
