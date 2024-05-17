// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/Actor.h"
#include "DynamicSpotLight.generated.h"

UCLASS()
class BEATEMUP_API ADynamicSpotLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicSpotLight();

	UPROPERTY(EditAnywhere)
		USpotLightComponent* Spotlight;

	UPROPERTY(EditAnywhere)
		AActor* TargetActor;

	UPROPERTY(EditAnywhere)
		bool bStartingSpotlight = false;

	UPROPERTY(EditAnywhere)
		FVector ActorOffset = FVector(0, 0, 1000);

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
