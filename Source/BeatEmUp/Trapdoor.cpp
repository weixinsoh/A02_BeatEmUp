// Fill out your copyright notice in the Description page of Project Settings.


#include "Trapdoor.h"

#include "BeatEmUpCharacter.h"

// Sets default values
ATrapdoor::ATrapdoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Door"));
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	LeftHinge = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Left Hinge"));
	RightHinge = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Right Hinge"));
	WidgetTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Widget Trigger"));
	InteractionPromptWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Prompt Widget Component"));

	LeftDoor->SetupAttachment(RootComponent);
	RightDoor->SetupAttachment(RootComponent);
	LeftHinge->SetupAttachment(RootComponent);
	RightHinge->SetupAttachment(RootComponent);
	WidgetTrigger->SetupAttachment(RootComponent);
	InteractionPromptWidgetComponent->SetupAttachment(RootComponent);
	
}

void ATrapdoor::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	LeftDoor->SetSimulatePhysics(true);
	RightDoor->SetSimulatePhysics(true);
	LeftDoor->AddImpulse(FVector::DownVector);
	RightDoor->AddImpulse(FVector::DownVector);
}

/**
 * This function set the interaction prompt UI to be visible
 * once the widget trigger overlapped with the player
 */
void ATrapdoor::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor))
	{
		InteractionPromptWidgetComponent->SetVisibility(true);
	}
}

/**
 * This function set the interaction prompt UI to be hidden
 * once the widget trigger end overlapped with the player
 */
void ATrapdoor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
	if (Player)
	{
		InteractionPromptWidgetComponent->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void ATrapdoor::BeginPlay()
{
	Super::BeginPlay();

	LeftDoor->SetSimulatePhysics(false);
	RightDoor->SetSimulatePhysics(false);

	// Create the interaction prompt widget and set the widget information
	PromptUI = Cast<UInteractionPromptUI>(CreateWidget(GetGameInstance(), InteractablePromptUIClass));
	PromptUI->ActionText->SetText(FText::FromString("Right Click to Use"));
	PromptUI->InteractableNameText->SetText(FText::FromString("Trapdoor"));
	InteractionPromptWidgetComponent->SetWidget(PromptUI);
	InteractionPromptWidgetComponent->SetVisibility(false);

	// dynamically delegate overlap functions once the widget trigger component overlapped/end overlapped with something
	WidgetTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATrapdoor::OnPlayerOverlap);
	WidgetTrigger->OnComponentEndOverlap.AddDynamic(this, &ATrapdoor::OnPlayerEndOverlap);
}
