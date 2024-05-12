// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "CoreMinimal.h"
#include "InteractionPromptUI.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Trapdoor.generated.h"

UCLASS()
class BEATEMUP_API ATrapdoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapdoor();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* LeftDoor;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* RightDoor;

	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* LeftHinge;

	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* RightHinge;

	virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere)
		UBoxComponent* WidgetTrigger;

	UPROPERTY(EditAnywhere)
		UWidgetComponent* InteractionPromptWidgetComponent;

	UPROPERTY(EditAnywhere)
		UInteractionPromptUI* PromptUI;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UInteractionPromptUI> InteractablePromptUIClass;

	UFUNCTION()
		void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
						 UPrimitiveComponent* OtherComponent,
						 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
