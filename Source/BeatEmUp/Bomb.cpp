// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb.h"

#include "BeatEmUpCharacter.h"
#include "Enemy.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ABomb::ABomb()
{
	WeaponName = "Bomb";
	WeaponDescription = "A Bomb";
	Damage = 100;
	AttackDistance = 1000;
	AttackSpeed = 100;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	Bomb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bomb"));
	Bomb->SetupAttachment(RootComponent);
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
	LaunchDirection *= 3;
	Bomb->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Bomb->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Bomb->SetSimulatePhysics(true);
	Bomb->SetPhysicsLinearVelocity(LaunchDirection * 1000);
	Bomb->SetNotifyRigidBodyCollision(true);
	Bomb->OnComponentHit.AddDynamic(this, &ABomb::OnHit);
}

void ABomb::BeginPlay()
{
	Super::BeginPlay();
	Bomb->OnComponentBeginOverlap.AddDynamic(this, &ABomb::OnOverlap);
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
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, Bomb->GetComponentLocation(), FRotator::ZeroRotator, FVector(3, 3, 3));
	TArray<FHitResult> Hits;
	TArray<AActor*> HitActors;
	FCollisionShape ExplosionSphere = FCollisionShape::MakeSphere(AttackDistance);
	bool bSweep = GetWorld()->SweepMultiByChannel(Hits, GetActorLocation(), GetActorLocation(), FQuat::Identity,
												  ECC_WorldDynamic, ExplosionSphere);
	if (bSweep)
	{
		for (FHitResult Hit : Hits)
		{
			if (!HitActors.Contains(Hit.GetActor()))
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
					HitMesh = Cast<UMeshComponent>(HitPlayer->GetMesh());
					HitPlayer->Ragdoll();
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
