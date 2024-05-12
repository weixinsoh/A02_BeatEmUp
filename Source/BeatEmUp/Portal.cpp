// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"

#include "BeatEmUpCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portal Mesh"));
	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Trigger"));
	PortalMesh->SetupAttachment(RootComponent);
	SphereTrigger->SetupAttachment(RootComponent);

	// Initialise boolean value
	bTeleported = false;
	
}

/**
 * Teleport the player to the boss level once player overlapped with it.
 */
void APortal::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                        UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bTeleported && Cast<ABeatEmUpCharacter>(OtherActor))
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("BossWorld"));
		bTeleported = true;
	}
}

/**
 * Invoke OnOverlap() function when the sphere trigger is ovelapped with something.
 */
void APortal::BeginPlay()
{
	Super::BeginPlay();
	
	SphereTrigger->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlap);
}

