// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeatEmUpCharacter.h"
#include "Bullet.h"
#include "DamageTextUI.h"
#include "HealthPack.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class BEATEMUP_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
		ABeatEmUpCharacter* Player;

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100;

	UPROPERTY(EditAnywhere)
		float CurrentHealth = MaxHealth;

	// Chase player and deal damage on hit
	UPROPERTY(EditAnywhere)
		float DamageToPlayer = 10;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UDamageTextUI> DamageTextClass;

	void DealDamage(float Damage);

	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);

	// Rag doll
	UPROPERTY(EditAnywhere)
	float RagdollTime = 5;

	FTimerHandle RagdollTimerHandle;
	
	virtual void Ragdoll();

	UFUNCTION()
		virtual void StopRagdoll();

	// Get freeze and unfreeze
	UPROPERTY(EditAnywhere)
		float FreezeTime = 5;

	FTimerHandle FreezeTimeHandle;
	
	UFUNCTION()
		void Freeze();

	UFUNCTION()
		void UnFreeze();

	// Shoot bullets
	UPROPERTY(EditAnywhere)
		TSubclassOf<ABullet> BulletClass;
	
	void Shoot(FVector Direction);

	UPROPERTY(EditAnywhere)
		int EXPAmount = 20;

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* DeathParticleClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AHealthPack> HealthPackClass;


};
