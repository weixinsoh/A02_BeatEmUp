// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "DynamicColorFloor.generated.h"

UCLASS()
class BEATEMUP_API ADynamicColorFloor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADynamicColorFloor();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UBoxComponent* WidgetTrigger;

	UFUNCTION()
		void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComponent,
							int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere)
		UMaterialInterface* EmissiveMaterialClass;

	UPROPERTY()
		UMaterialInstanceDynamic* EmissiveMaterialInstance;

	UPROPERTY()
		float EmissiveStrength = 50;

	UPROPERTY(EditAnywhere)
		TArray<FLinearColor> Colors;

	UPROPERTY(EditAnywhere)
		float ColorChangeSpeed = 2;

	bool bIsOverlapping;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
