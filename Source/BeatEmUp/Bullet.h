// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class BEATEMUP_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:	

	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		float MovementSpeed = 200;

	UPROPERTY(EditAnywhere)
		float Damage = 5;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere)
		UNiagaraSystem* BulletTrailSystem;

	UPROPERTY(EditAnywhere)
		UNiagaraComponent* BulletTrailComponent;
	
	UPROPERTY(EditAnywhere)
		UMaterialInterface* MaterialClass;

	UPROPERTY()
		UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(EditAnywhere)
		float ChangeSpeed = 2;

	UPROPERTY(EditAnywhere)
		float ParticleMaxVelocity = 800;

	UPROPERTY(EditAnywhere)
		float ParticleMinVelocity = 0;

	UPROPERTY(EditAnywhere)
		float Velocity = ParticleMinVelocity;

	UPROPERTY(EditAnywhere)
		float ParticleMaxLifeTime = 1;

	UPROPERTY(EditAnywhere)
		float ParticleMinLifeTime = 0;

	UPROPERTY(EditAnywhere)
		float LifeTime = ParticleMinLifeTime;

	float CurrentOpacity = 1;
	bool bIsHit;

	UPROPERTY()
	UNiagaraComponent* Test;
};
