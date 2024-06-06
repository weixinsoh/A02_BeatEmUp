// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicColorFloor.h"

#include "BeatEmUpCharacter.h"

// Sets default values
ADynamicColorFloor::ADynamicColorFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	WidgetTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Widget Trigger"));
	WidgetTrigger->SetupAttachment(RootComponent);

	bIsOverlapping = false;
}

/**
 * This function is used to make the floor glowing in random color once player begin overlaps with it. 
 */
void ADynamicColorFloor::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
	if (EmissiveMaterialInstance && Player)
	{
		bIsOverlapping = true;
		EmissiveMaterialInstance->SetScalarParameterValue("EmissiveStrength", EmissiveStrength);
	}
}

/**
 * This function is used to remove the glowing once player ends overlaps with it. 
 */
void ADynamicColorFloor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
	if (EmissiveMaterialInstance && Player)
	{
		bIsOverlapping = false;
		EmissiveMaterialInstance->SetScalarParameterValue("EmissiveStrength", 0.0f);
	}
}

// Called when the game starts or when spawned
void ADynamicColorFloor::BeginPlay()
{
	Super::BeginPlay();

	WidgetTrigger->OnComponentBeginOverlap.AddDynamic(this, &ADynamicColorFloor::OnPlayerOverlap);
	WidgetTrigger->OnComponentEndOverlap.AddDynamic(this, &ADynamicColorFloor::OnPlayerEndOverlap);

	if (EmissiveMaterialClass)
	{
		EmissiveMaterialInstance = UMaterialInstanceDynamic::Create(EmissiveMaterialClass, this);
		Mesh->SetMaterial(0, EmissiveMaterialInstance);
	}
}

void ADynamicColorFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("check: %d"), bIsOverlapping);
	if (bIsOverlapping && EmissiveMaterialInstance)
	{
		float Time = GetWorld()->GetTimeSeconds() * ColorChangeSpeed;

		// Determine which two colors to interpolate between
		int32 ColorIndex1 = FMath::FloorToInt(Time) % Colors.Num();
		int32 ColorIndex2 = (ColorIndex1 + 1) % Colors.Num();

		// Calculate the interpolation alpha
		float Alpha = FMath::Frac(Time);

		// Interpolate between the two colors
		FLinearColor CurrentColor = FLinearColor::LerpUsingHSV(Colors[ColorIndex1], Colors[ColorIndex2], Alpha);

		// Update the emissive material instance
		EmissiveMaterialInstance->SetVectorParameterValue("BaseColor", CurrentColor);
	}
}

