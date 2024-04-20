// Fill out your copyright notice in the Description page of Project Settings.


#include "Trapdoor.h"

#include "BeatEmUpCharacter.h"

// Sets default values
ATrapdoor::ATrapdoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ATrapdoor::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor))
	{
		InteractionPromptWidgetComponent->SetVisibility(true);
	}
}

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

	PromptUI = Cast<UInteractionPromptUI>(CreateWidget(GetGameInstance(), InteractablePromptUIClass));
	PromptUI->ActionText->SetText(FText::FromString("Right Click to Use"));
	PromptUI->InteractableNameText->SetText(FText::FromString("Trapdoor"));
	InteractionPromptWidgetComponent->SetWidget(PromptUI);
	InteractionPromptWidgetComponent->SetVisibility(false);
	
	WidgetTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATrapdoor::OnPlayerOverlap);
	WidgetTrigger->OnComponentEndOverlap.AddDynamic(this, &ATrapdoor::OnPlayerEndOverlap);
}

// Called every frame
void ATrapdoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

