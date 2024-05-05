// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Axe.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AAxe : public AWeapon
{
	GENERATED_BODY()

public:

	AAxe();
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AxeHead;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AxeHandler;
	
	UPROPERTY(EditAnywhere)
	float Damage = 150;
	
	UPROPERTY(EditAnywhere)
	float AttackDistance = 500;
	
	UPROPERTY(EditAnywhere)
	float AttackSpeed = 50;

	virtual void UseWeapon(ACharacter* Character) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						 UPrimitiveComponent* OtherComponent,
						 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);
};
