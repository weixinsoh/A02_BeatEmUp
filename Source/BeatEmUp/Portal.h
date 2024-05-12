// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

UCLASS()
class BEATEMUP_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PortalMesh;

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereTrigger;
	
	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						 UPrimitiveComponent* OtherComponent,
						 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bTeleported;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
