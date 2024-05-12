// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"

#include "BeatEmUpCharacter.h"
#include "Enemy.h"

/**
 * Constructor for axe weapon.
 */
AAxe::AAxe()
{
	// Set weapon info
	WeaponName = "Axe";
	WeaponDescription = "A melee tool featuring a sharp blade attached to a sturdy handle. Designed for close-quarters combat, it delivers powerful and sweeping strikes. ";
	Damage = 10;
	AttackDistance = 400;
	AttackSpeed = 50;

	// Create component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	AxeHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe Head"));
	AxeHandler = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Axe Handler"));
	AxeHead->SetupAttachment(RootComponent);
	AxeHandler->SetupAttachment(RootComponent);

}

/**
 * Begin play function which dynamically delegate OnOverlap() function
 * once the axe head is overlapped with something.
 */
void AAxe::BeginPlay()
{
	Super::BeginPlay();
	AxeHead->OnComponentBeginOverlap.AddDynamic(this, &AAxe::OnOverlap);
}

/**
 * Deal damage to the hit target. If the hit target is enemy, add impulse to it. 
 * @param Character The character who use this weapon.
 */
void AAxe::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);

	// Detect hit target by performing Multi Sweep using cube collision
	TArray<FHitResult> HitResults;
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * AttackDistance;
	const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(AttackDistance));
	const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, End, End, GetActorQuat(), ECC_WorldDynamic, CubeShape);
	TArray<AActor*> HitThisPunch;

	// Perform attack effect on every hit target
	for(FHitResult HitResult : HitResults)
	{
		if (HitResult.GetActor() != Character && !HitThisPunch.Contains(HitResult.GetActor()))
		{
			HitThisPunch.Add(HitResult.GetActor());
			AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor());
			ABeatEmUpCharacter* HitPlayer = Cast<ABeatEmUpCharacter>(HitResult.GetActor());
			if (HitEnemy)
			{
				// Add impulse to the enemy and deal damage
				HitEnemy->Ragdoll();
				FVector LaunchDirection = HitEnemy->GetActorLocation() - GetActorLocation();
				LaunchDirection.Normalize();
				LaunchDirection *= 3;
				LaunchDirection += FVector::UpVector;
				HitEnemy->GetMesh()->AddImpulse(LaunchDirection * HitForce);
				HitEnemy->DealDamage(Damage);
				
			} else if (HitPlayer)
			{
				// Deal damage to the player
				HitPlayer->DealDamage(Damage);
			}
		}
	}
}

