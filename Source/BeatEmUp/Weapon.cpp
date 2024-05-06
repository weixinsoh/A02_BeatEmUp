// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Enemy.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponName = FString("Default name");
	WeaponDescription = FString("Default description");

	bIsPickedUp = false;
}

void AWeapon::SetName(FString Name)
{
	WeaponName = Name;
}

void AWeapon::SetDescription(FString Description)
{
	WeaponDescription = Description;
}

void AWeapon::SetIcon(UTexture2D* Icon)
{
	WeaponIcon = Icon;
}

FString AWeapon::GetName()
{
	return WeaponName;
}

FString AWeapon::GetDescription()
{
	return WeaponDescription;
}

void AWeapon::UseWeapon(ACharacter* Character)
{
}

void AWeapon::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                             UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                             const FHitResult& SweepResult)
{
	if (!bIsPickedUp)
	{
		ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
		if (Player)
		{
			Player->PickUp(this);
			SetActorHiddenInGame(true);
		}
	}
}


// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
