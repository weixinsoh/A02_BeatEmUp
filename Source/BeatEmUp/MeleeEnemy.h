// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemyWeaponBTController.h"
#include "CoreMinimal.h"
#include "Axe.h"
#include "Bomb.h"
#include "Enemy.h"
#include "MeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AMeleeEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	AMeleeEnemy();
	virtual void BeginPlay() override;
	virtual void Ragdoll() override;
	virtual void StopRagdoll() override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit) override;

	void UseLeftWeapon();
	void UseRightWeapon();
	void SetMovementSpeed(float Speed);
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABomb> BombClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AAxe> AxeClass;

	UPROPERTY(EditAnywhere)
		AWeapon* LeftWeapon;

	UPROPERTY(EditAnywhere)
		AWeapon* RightWeapon;

};
