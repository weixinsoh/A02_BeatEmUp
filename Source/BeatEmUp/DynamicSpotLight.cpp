// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicSpotLight.h"

#include "Enemy.h"

// Sets default values
ADynamicSpotLight::ADynamicSpotLight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Spotlight"));
    RootComponent = Spotlight;


}

// Called when the game starts or when spawned
void ADynamicSpotLight::BeginPlay()
{
	Super::BeginPlay();
	
	if(bStartingSpotlight)
	{
		TargetActor = GetWorld()->GetFirstPlayerController()->GetPawn(); }
	}

// Called every frame
void ADynamicSpotLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(TargetActor)
	{
		 FVector TargetDirection = TargetActor->GetActorLocation() - GetActorLocation();
		 SetActorRotation(TargetDirection.Rotation());
		 if(!bStartingSpotlight)
		 {
		 	AEnemy* Enemy = Cast<AEnemy>(TargetActor);
		 	if(Enemy)
		 	{
		 		SetActorLocation(Enemy->GetMesh()->GetComponentLocation() + ActorOffset);
		 		TargetDirection = Enemy->GetMesh()->GetComponentLocation() - GetActorLocation();
		 		SetActorRotation(TargetDirection.Rotation());
		 		Spotlight->SetLightColor(FLinearColor::LerpUsingHSV(HealthyColour, DeadColour, (1-(float)Enemy->CurrentHealth / Enemy->MaxHealth)));
		 	}
		 	else
		 	{
		 		SetActorLocation(TargetActor->GetActorLocation()+ ActorOffset);
		 	}
		 }
		else
		{
			ABeatEmUpCharacter* Character = Cast<ABeatEmUpCharacter>(TargetActor);
			if(Character)
			{
				Spotlight->SetLightColor(FLinearColor::LerpUsingHSV(HealthyColour, DeadColour, (1-(float)Character->CurrentHealth / Character->MaxHealth)));
			}
		}
	}

	
}

