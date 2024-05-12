// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"

#include "BeatEmUpCharacter.h"
#include "Enemy.h"
#include "MeleeEnemy.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

/**
 * Constructor for bomb.
 */
ABomb::ABomb()
{
	// Set weapon info
	WeaponName = "Bomb";
	WeaponDescription = "An explosive device. When activated, it releases a powerful blast, impacting a wide area and potentially harming enemies within its vicinity. ";
	Damage = 20;
	AttackDistance = 800;
	AttackSpeed = 100;

	// Create component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb"));
	BombMesh->SetupAttachment(RootComponent);
}

/**
 * Throw the bomb in projectile motion and collide with the world static and dynamic component. 
 * @param Character The character who use this weapon.
 */
void ABomb::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// Detach from the player and remove it from inventory
	if (ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(Character))
	{
		Player->DropWeapon();
	}

	// Calculate the launch direction
	FVector LaunchDirection = Character->GetActorForwardVector();
	LaunchDirection.Normalize();
	LaunchDirection *= 2;
	if (Cast<AMeleeEnemy>(Character))
	{
		LaunchDirection *= 3;
		LaunchDirection += FVector::UpVector * 5;
	}
	else
	{
		LaunchDirection += FVector::UpVector;
	}

	// Set collision settings
	BombMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BombMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	BombMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// Launch the bomb
	BombMesh->SetSimulatePhysics(true);
	BombMesh->SetPhysicsLinearVelocity(LaunchDirection * AttackSpeed);
	BombMesh->SetEnableGravity(true);
	BombMesh->SetNotifyRigidBodyCollision(true);
	BombMesh->OnComponentHit.AddDynamic(this, &ABomb::OnHit);
}

/**
 * Begin play function which dynamically delegate OnOverlap() function
 * once the bomb is overlapped with something.
 */
void ABomb::BeginPlay()
{
	Super::BeginPlay();
	BombMesh->OnComponentBeginOverlap.AddDynamic(this, &ABomb::OnOverlap);
}

/**
 * Explode once the bomb hitted something.
 * Deal demage to all the hit targets within the explosion area. 
 */
void ABomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
                  const FHitResult& Hit)
{
	// Emit explosion particle
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, BombMesh->GetComponentLocation(),
	                                         FRotator::ZeroRotator, FVector(3, 3, 3));

	// Detect hit target by performing Multi Sweep using sphere collision
	TArray<FHitResult> Hits;
	TArray<AActor*> HitActors;
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(AttackDistance);
	bool bSweep = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity,
	                                              ECC_WorldDynamic, ExplosionSphere);

	// Perform attack effect on every hit target
	if (bSweep)
	{
		for (FHitResult Hit : Hits)
		{
			if (Hit.GetActor() != PickedUpCharacter && !HitActors.Contains(Hit.GetActor()))
			{
				HitActors.Add(Hit.GetActor());
				UMeshComponent* HitMesh;
				AEnemy* HitEnemy = Cast<AEnemy>(Hit.GetActor());
				ABeatEmUpCharacter* HitPlayer = Cast<ABeatEmUpCharacter>(Hit.GetActor());

				if (HitEnemy)
				{
					HitMesh = Cast<UMeshComponent>(HitEnemy->GetMesh());
					HitEnemy->Ragdoll();
				}
				else if (HitPlayer)
				{
					HitPlayer->DealDamage(Damage);
					Destroy();
					return;
				}
				else
				{
					HitMesh = Cast<UMeshComponent>(Hit.GetActor()->GetRootComponent());
				}

				if (HitMesh)
				{
					HitMesh->AddRadialImpulse(GetActorLocation(), AttackDistance, -ExplosionForce, RIF_Linear, true);
				}
			}
		}
	}
	Destroy();
}
