// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityWell.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "InteractionPromptUI.h"

// Sets default values
AGravityWell::AGravityWell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	WidgetTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Widget Trigger"));
	WidgetTrigger->SetupAttachment(RootComponent);

	InteractionPromptWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Interaction Prompt Widget Component"));
	InteractionPromptWidgetComponent->SetupAttachment(RootComponent);

}

void AGravityWell::Activate()
{
	TArray<FHitResult> Hits;
	TArray<AActor*> HitActors;
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(SweepSize);
	bool bSweep = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity,
	                                              ECC_WorldDynamic, ExplosionSphere);
	if (bSweep)
	{
		for (FHitResult Hit : Hits)
		{
			if (!HitActors.Contains(Hit.GetActor()))
			{
				HitActors.Add(Hit.GetActor());
				UMeshComponent* HitMesh;
				AEnemy* HitEnemy = Cast<AEnemy>(Hit.GetActor());
				if (HitEnemy)
				{
					HitMesh = Cast<UMeshComponent>(HitEnemy->GetMesh());
					HitEnemy->Ragdoll();
				}
				else
				{
					HitMesh = Cast<UMeshComponent>(Hit.GetActor()->GetRootComponent());
				}

				if (HitMesh)
				{
					switch (GravityType)
					{
					case EGravityType::GE_PULL:
						HitMesh->AddRadialImpulse(GetActorLocation(), SweepSize, Force, RIF_Linear, true);
						break;
					case EGravityType::GE_PUSH:
						HitMesh->AddRadialImpulse(GetActorLocation(), SweepSize, -Force, RIF_Linear, true);
						break;
					case EGravityType::GE_ON:
						if (!HitEnemy)
						{
							HitMesh->SetEnableGravity(false);
						}
						break;
					default:
						UE_LOG(LogTemp, Warning, TEXT("No Gravity Type! How did this happen!?"));
					}
				}
			}
		}
	}
}

void AGravityWell::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	Activate();
}

/**
 * This function set the interaction prompt UI to be visible
 * once the widget trigger overlapped with the player
 */
void AGravityWell::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlapped:%s"), *OtherActor->GetClass()->GetName());

	if (ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor))
	{
		InteractionPromptWidgetComponent->SetVisibility(true);
	}
}

/**
 * This function set the interaction prompt UI to be hidden
 * once the widget trigger end overlapped with the player
 */
void AGravityWell::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
	if (Player)
	{
		InteractionPromptWidgetComponent->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void AGravityWell::BeginPlay()
{
	Super::BeginPlay();

	// Create the interaction prompt widget and set the widget information
	PromptUI = Cast<UInteractionPromptUI>(CreateWidget(GetGameInstance(), InteractablePromptUIClass));
	if (PromptUI)
	{
		PromptUI->ActionText->SetText(FText::FromString("Right Click to Use"));
		PromptUI->InteractableNameText->SetText(FText::FromString("Gravity Well"));
		InteractionPromptWidgetComponent->SetWidget(PromptUI);
		InteractionPromptWidgetComponent->SetVisibility(false);
	}

	// dynamically delegate overlap functions once the widget trigger component overlapped/end overlapped with something
	WidgetTrigger->OnComponentBeginOverlap.AddDynamic(this, &AGravityWell::OnPlayerOverlap);
	WidgetTrigger->OnComponentEndOverlap.AddDynamic(this, &AGravityWell::OnPlayerEndOverlap);
}

// Called every frame
void AGravityWell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
