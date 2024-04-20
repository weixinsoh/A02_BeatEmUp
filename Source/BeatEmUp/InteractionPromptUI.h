// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InteractionPromptUI.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API UInteractionPromptUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		UImage* KeyImage;
	
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ActionText;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* InteractableNameText;

	FVector2D DisplayLocation;

};
