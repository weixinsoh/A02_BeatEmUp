// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicColorFloor.h"

#include "BeatEmUpCharacter.h"

// Sets default values
ADynamicColorFloor::ADynamicColorFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	WidgetTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Widget Trigger"));
	WidgetTrigger->SetupAttachment(RootComponent);
}

void ADynamicColorFloor::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
	if (EmissiveMaterialInstance && Player)
	{
		EmissiveMaterialInstance->SetVectorParameterValue("BaseColor", FLinearColor::MakeRandomColor());
		EmissiveMaterialInstance->SetScalarParameterValue("EmissiveStrength", EmissiveStrength);
	}
}

void ADynamicColorFloor::OnPlayerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
	if (EmissiveMaterialInstance && Player)
	{
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

