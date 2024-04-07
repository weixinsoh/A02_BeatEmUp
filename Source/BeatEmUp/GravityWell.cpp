// Fill out your copyright notice in the Description page of Project Settings.


#include "GravityWell.h"

#include "Enemy.h"

// Sets default values
AGravityWell::AGravityWell()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
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
		for (FHitResult Hit: Hits)
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
			}
		}
	}
}

void AGravityWell::Interact_Implementation()
{
	IInteractable::Interact_Implementation();

	Activate();
}

// Called when the game starts or when spawned
void AGravityWell::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGravityWell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
