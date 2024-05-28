// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingSpotLight.h"

// Sets default values
ARotatingSpotLight::ARotatingSpotLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	LeftSpotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("LeftSpotlight"));
	RightSpotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("RightSpotlight"));
	MovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement Component"));
	LeftSpotlight->SetupAttachment(RootComponent);
	RightSpotlight->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ARotatingSpotLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotatingSpotLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

