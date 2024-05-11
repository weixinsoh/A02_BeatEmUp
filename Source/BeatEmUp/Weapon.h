// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class BEATEMUP_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	UPROPERTY()
		FString WeaponName;

	UPROPERTY()
		FString WeaponDescription;

	UPROPERTY()
		UTexture2D* WeaponIcon;

	UPROPERTY(EditAnywhere)
		float Damage;
	
	UPROPERTY(EditAnywhere)
		float AttackDistance;
	
	UPROPERTY(EditAnywhere)
		float AttackSpeed;
	
	UPROPERTY()
		ACharacter* PickedUpCharacter;

	void SetName(FString Name);
	void SetDescription(FString Description);
	void SetIcon(UTexture2D* Icon);
	FString GetName();
	FString GetDescription();

	virtual void UseWeapon(ACharacter* Character);

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						 UPrimitiveComponent* OtherComponent,
						 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
