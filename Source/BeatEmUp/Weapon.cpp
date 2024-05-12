// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Enemy.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponName = FString("Default name");
	WeaponDescription = FString("Default description");
}

/**
 * Set the name of the weapon.
 * @param Name the name of the weapon.
 */
void AWeapon::SetName(FString Name)
{
	WeaponName = Name;
}

/**
 * Set the description of the weapon.
 * @param Description the description of the weapon.
 */
void AWeapon::SetDescription(FString Description)
{
	WeaponDescription = Description;
}

/**
 * Set the display icon of the weapon.
 * @param Icon the display icon.
 */
void AWeapon::SetIcon(UTexture2D* Icon)
{
	WeaponIcon = Icon;
}

/**
 * Returns the name of the weapon.
 */
FString AWeapon::GetName()
{
	return WeaponName;
}

/**
 * Returns the description of the weapon. 
 */
FString AWeapon::GetDescription()
{
	return WeaponDescription;
}

/**
 * Perform the attack action and apply effects on the hit targets.
 * @param Character The character who use this weapon.
 */
void AWeapon::UseWeapon(ACharacter* Character)
{
}

/**
 * This function calls player to pick up the weapon once they are overlapped.
 * It hides the weapon from the game world and set its owner as the player. 
 */
void AWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	if (PickedUpCharacter == nullptr)
	{
		ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
		if (Player)
		{
			Player->PickUp(this);
		}
	}
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}
