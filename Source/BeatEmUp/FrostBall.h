// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "FrostBall.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AFrostBall : public AWeapon
{
	GENERATED_BODY()

public:
	AFrostBall();
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere)
		UParticleSystem* ExplosionParticle;

	virtual void UseWeapon(ACharacter* Character) override;

	virtual void BeginPlay() override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
			const FHitResult& Hit);
	

};
