// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossUI.h"
#include "Enemy.h"
#include "Stick.h"
#include "BossEnemy.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API ABossEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	ABossEnemy();
	virtual void BeginPlay() override;
	virtual void Ragdoll() override;
	virtual void StopRagdoll() override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit) override;
	virtual void DealDamage(float Damage) override;
	
	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	void Attack();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AStick> StickClass;

	UPROPERTY(EditAnywhere)
	APawn* TargetPlayer;

	UPROPERTY(EditAnywhere)
	UBossUI* BossUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBossUI> BossUIClass;
};
