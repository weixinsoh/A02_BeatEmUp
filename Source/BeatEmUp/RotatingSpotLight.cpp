// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingSpotLight.h"

#include "Kismet/GameplayStatics.h"

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
	MovementComponent->RotationRate = FRotator(0, 0, RotationRate);
	LeftSpotlight->SetupAttachment(RootComponent);
	RightSpotlight->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void ARotatingSpotLight::BeginPlay()
{
	Super::BeginPlay();
	BossEnemy = Cast<ABossEnemy>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossEnemy::StaticClass()));
}

// Called every frame
void ARotatingSpotLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (BossEnemy)
	{
		if (MaterialClass)
		{
			MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
			MaterialInstance->SetVectorParameterValue("Colour", FLinearColor::LerpUsingHSV(HealthyColour, DeadColour, (1-(float)BossEnemy->CurrentHealth / BossEnemy->MaxHealth)));
			MaterialInstance->SetScalarParameterValue("Frequency", FMath::Lerp(0.1, 500, DeltaTime * 5));
			Mesh->SetMaterial(0, MaterialInstance);
			LeftSpotlight->SetLightFunctionMaterial(MaterialInstance);
			RightSpotlight->SetLightFunctionMaterial(MaterialInstance);
			LeftSpotlight->SetLightColor(FLinearColor::LerpUsingHSV(HealthyColour, DeadColour, (1-(float)BossEnemy->CurrentHealth / BossEnemy->MaxHealth)));
			RightSpotlight->SetLightColor(FLinearColor::LerpUsingHSV(HealthyColour, DeadColour, (1-(float)BossEnemy->CurrentHealth / BossEnemy->MaxHealth)));
		}
	}
}

