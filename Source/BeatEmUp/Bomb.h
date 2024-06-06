// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Bomb.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API ABomb : public AWeapon
{
	GENERATED_BODY()

public:
	ABomb();
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BombMesh;

	UPROPERTY(EditAnywhere)
		float ExplosionForce = 80000;

	virtual void UseWeapon(ACharacter* Character) override;

	virtual void BeginPlay() override;
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
		UParticleSystem* ExplosionParticle;
};
