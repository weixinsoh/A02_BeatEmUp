// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"

#include "BeatEmUpCharacter.h"
#include "Enemy.h"

AAxe::AAxe()
{
	WeaponName = "Axe";
	WeaponDescription = "An Axe";

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	AxeHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe Head"));
	AxeHandler = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe Handler"));
	AxeHead->SetupAttachment(RootComponent);
	AxeHandler->SetupAttachment(RootComponent);

}

void AAxe::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);

	TArray<FHitResult> HitResults;
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * AttackDistance;
	const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(AttackDistance));
	const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, End, End, GetActorQuat(), ECC_WorldDynamic, CubeShape);

	TArray<AActor*> HitThisPunch;
	for(FHitResult HitResult : HitResults)
	{
		if (HitResult.GetActor() != this && !HitThisPunch.Contains(HitResult.GetActor()))
		{
			HitThisPunch.Add(HitResult.GetActor());
			AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor());
			if (HitEnemy)
			{
				HitEnemy->Ragdoll();
				FVector LaunchDirection = HitEnemy->GetActorLocation() - GetActorLocation();
				LaunchDirection.Normalize();
				LaunchDirection *= 3;
				LaunchDirection += FVector::UpVector;
				//HitEnemy->GetMesh()->AddImpulse(LaunchDirection * PunchForce);
				HitEnemy->DealDamage(Damage);
			}
		}
	}
}

void AAxe::BeginPlay()
{
	Super::BeginPlay();
	AxeHead->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnOverlap);
}

void AAxe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAxe::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
}

void AAxe::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
                 const FHitResult& Hit)
{
	//bHit = true;
	ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(OtherActor);
	if (Player)
	{
		Player->DealDamage(Damage);
	} else
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			Enemy->DealDamage(Damage);
		}
	}

}
