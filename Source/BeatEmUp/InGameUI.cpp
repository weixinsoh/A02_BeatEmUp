// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameUI.h"

#include "BeatEmUpCharacter.h"

void UInGameUI::UpdateValues()
{
	HealthBar->SetPercent((float)Player->CurrentHealth/Player->MaxHealth);
	CurrentHealthText->SetText(FText::FromString(FString::FromInt(Player->CurrentHealth)));
	float PunchTimePercentage = 0;
	if(GetWorld()->GetTimerManager().TimerExists(Player->PunchTimerHandle))
	{
		PunchTimePercentage = GetWorld()->GetTimerManager().GetTimerRemaining(Player->PunchTimerHandle) / Player->PunchCooldown;
	}
	PunchCoolDown->SetPercent(1 - PunchTimePercentage);
	MaxHealthText->SetText(FText::FromString(FString::SanitizeFloat(Player->MaxHealth)));
	EXPBar->SetPercent((float)Player->CurrentEXP / Player->EXPToLevel);
}
