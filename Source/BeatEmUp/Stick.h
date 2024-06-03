// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BeatEmUpCharacter.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Stick.generated.h"

UCLASS()
class BEATEMUP_API AStick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStick();
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	APawn* TargetPlayer;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* MovementComponent;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);

	UFUNCTION()
	void DelayProject();
	
	UFUNCTION()
	void Project();

	FTimerHandle ProjectTimerHandle;

	UPROPERTY(EditAnywhere)
	float ProjectTime = 1;

	UPROPERTY(EditAnywhere)
	bool bDestroy;
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaterialClass;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;

	float CurrentOpacity = 1;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
