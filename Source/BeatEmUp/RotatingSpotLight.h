// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BossEnemy.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "RotatingSpotLight.generated.h"

UCLASS()
class BEATEMUP_API ARotatingSpotLight : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARotatingSpotLight();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		USpotLightComponent* LeftSpotlight;

	UPROPERTY(EditAnywhere)
		USpotLightComponent* RightSpotlight;

	UPROPERTY(EditAnywhere)
		URotatingMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
		float RotationRate = 80;

	UPROPERTY(EditAnywhere)
		ABossEnemy* BossEnemy;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* MaterialClass;

	UPROPERTY()
		UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(EditAnywhere)
		FLinearColor HealthyColour;

	UPROPERTY(EditAnywhere)
		FLinearColor DeadColour;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
