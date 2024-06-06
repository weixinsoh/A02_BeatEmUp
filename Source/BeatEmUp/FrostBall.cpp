// Fill out your copyright notice in the Description page of Project Settings.


#include "FrostBall.h"

#include "Enemy.h"
#include "MeleeEnemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

/**
 * Constructor for frost ball.
 */
AFrostBall::AFrostBall()
{
	// Set weapon info
	WeaponName = "Frost Ball";
	WeaponDescription = "A magical projectile that freezes enemies upon impact, immobilizing them temporarily. ";
	AttackDistance = 800;
	AttackSpeed = 200;
	Damage = 0;

	// Create component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	BallMesh->SetupAttachment(RootComponent);

}

/**
 * Throw the frost ball in projectile motion and collide with the world static and dynamic component. 
 * @param Character The character who use this weapon.
 */
void AFrostBall::UseWeapon(ACharacter* Character)
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
	LaunchDirection *= 3;

	// Set collision settings
	BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BallMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	// Launch the frost ball
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetPhysicsLinearVelocity(LaunchDirection * AttackSpeed);
	BallMesh->SetEnableGravity(true);
	BallMesh->SetNotifyRigidBodyCollision(true);
	BallMesh->OnComponentHit.AddDynamic(this, &AFrostBall::OnHit);
}

/**
 * Begin play function which dynamically delegate OnOverlap() function
 * once the ball is overlapped with something.
 */
void AFrostBall::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentBeginOverlap.AddDynamic(this, &AFrostBall::OnOverlap);
}

/**
 * Explode once the ball hitted something.
 * Freeze the movement of the hit targets for a period of time.
 */
void AFrostBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	// Emit explosion particle
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, BallMesh->GetComponentLocation(), FRotator::ZeroRotator, FVector(3, 3, 3));

	// Detect hit target by performing Multi Sweep using sphere collision
	TArray<FHitResult> Hits;
	TArray<AActor*> HitActors;
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(AttackDistance);
	bool bSweep = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity,
												  ECC_WorldDynamic, ExplosionSphere);

	// Freeze every hit target
	if (bSweep)
	{
		for (FHitResult Hit : Hits)
		{
			if (Hit.GetActor() != PickedUpCharacter && !HitActors.Contains(Hit.GetActor()))
			{
				HitActors.Add(Hit.GetActor());
				AEnemy* HitEnemy = Cast<AEnemy>(Hit.GetActor());
				if (HitEnemy)
				{
					HitEnemy->Ragdoll();
					HitEnemy->Freeze();
				}
			}
		}
	}
	Destroy();
}
