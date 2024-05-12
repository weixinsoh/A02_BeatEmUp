// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
		UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	UFUNCTION()
		void ContinueButtonClick();

	UFUNCTION()
		void SaveButtonClick();

	UFUNCTION()
		void QuitButtonClick();
};
