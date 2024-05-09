// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"

#include "BeatEmUpCharacter.h"
#include "Enemy.h"
#include "MeleeEnemy.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABomb::ABomb()
{
	WeaponName = "Bomb";
	WeaponDescription = "A Bomb";
	Damage = 5;
	AttackDistance = 800;
	AttackSpeed = 100;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb"));
	BombMesh->SetupAttachment(RootComponent);
}

void ABomb::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(Character))
	{
		Player->DropWeapon();
	}
	FVector LaunchDirection = Character->GetActorForwardVector();
	LaunchDirection.Normalize();
	LaunchDirection *= 2;
	if (Cast<AMeleeEnemy>(Character))
	{
		LaunchDirection *= 3;
		LaunchDirection += FVector::UpVector * 5;
	} else
	{
		LaunchDirection += FVector::UpVector;
	}
	UE_LOG(LogTemp, Warning, TEXT("launch vector: %s"), *LaunchDirection.ToString());
	BombMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BombMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BombMesh->SetSimulatePhysics(true);
	BombMesh->SetPhysicsLinearVelocity(LaunchDirection * AttackSpeed);
	BombMesh->SetEnableGravity(true);
	BombMesh->SetNotifyRigidBodyCollision(true);
	BombMesh->OnComponentHit.AddDynamic(this, &ABomb::OnHit);
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	BombMesh->OnComponentBeginOverlap.AddDynamic(this, &ABomb::OnOverlap);
}

void ABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABomb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
}

void ABomb::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, BombMesh->GetComponentLocation(), FRotator::ZeroRotator, FVector(3, 3, 3));
	TArray<FHitResult> Hits;
	TArray<AActor*> HitActors;
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(AttackDistance);
	bool bSweep = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity,
												  ECC_WorldDynamic, ExplosionSphere);
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
				} else if (HitPlayer)
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
