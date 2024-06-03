// Fill out your copyright notice in the Description page of Project Settings.


#include "BossUI.h"
#include "BossEnemy.h"

void UBossUI::UpdateValues()
{
	HealthBar->SetPercent((float)BossEnemy->CurrentHealth/BossEnemy->MaxHealth);
	CurrentHealthText->SetText(FText::FromString(FString::FromInt(BossEnemy->CurrentHealth)));
	MaxHealthText->SetText(FText::FromString(FString::FromInt(BossEnemy->MaxHealth)));
}
