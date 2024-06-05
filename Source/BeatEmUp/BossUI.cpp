// Fill out your copyright notice in the Description page of Project Settings.


#include "BossUI.h"
#include "BossEnemy.h"

/**
 * This function is used to update the health bar widget once the boss enemy's health changes. 
 */
void UBossUI::UpdateValues()
{
	HealthBar->SetPercent((float)BossEnemy->CurrentHealth/BossEnemy->MaxHealth);
	CurrentHealthText->SetText(FText::FromString(FString::FromInt(BossEnemy->CurrentHealth)));
	MaxHealthText->SetText(FText::FromString(FString::FromInt(BossEnemy->MaxHealth)));
}
