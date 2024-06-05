// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTrigger.h"

#include "BeatEmUpCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelTrigger::ALevelTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
    RootComponent = TriggerVolume;

}

void ALevelTrigger::StartLevelStreaming(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 BodyIndex, bool bSweep, const FHitResult& Hit)
{
	if(Cast<ABeatEmUpCharacter>(OtherActor))
	{
		ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if(bLoadLevel && Player && Player->NumEnemiesDefeated >= Player->NumEnemiesToDefeat)
		{
			Player->bLevelLoaded = true;
			UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, FLatentActionInfo());

			// Spawn boss enemy when boss level is loaded
			FVector Location = Player->GetActorLocation() + Player->GetActorForwardVector() * 1000;
			FRotator Rotation = Player->GetActorRotation();
			Rotation.Yaw += 180;
			GetWorld()->SpawnActor(BossEnemyClass, &Location, &Rotation);
		}
		else
		{
			UGameplayStatics::UnloadStreamLevel(GetWorld(), LevelToLoad, FLatentActionInfo(), true);
		}
	}
}

// Called when the game starts or when spawned
void ALevelTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTrigger::StartLevelStreaming);
	UE_LOG(LogTemp, Warning, TEXT("Loadlevel"));

}

// Called every frame
void ALevelTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

