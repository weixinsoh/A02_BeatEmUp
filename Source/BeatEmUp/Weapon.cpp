// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "BeatEmUpCharacter.h"

UWeapon::UWeapon()
{
	WeaponName = FString("Default name");
	WeaponDescription = FString("Default description");
}

void UWeapon::SetName(FString Name)
{
	WeaponName = Name;
}

void UWeapon::SetDescription(FString Description)
{
	WeaponDescription = Description;
}

FString UWeapon::GetName()
{
	return WeaponName;
}

FString UWeapon::GetDescription()
{
	return WeaponDescription;
}

void UWeapon::UseWeapon(ACharacter* Character)
{
}
