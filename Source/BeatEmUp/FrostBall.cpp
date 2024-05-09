// Fill out your copyright notice in the Description page of Project Settings.


#include "FrostBall.h"

#include "Enemy.h"
#include "MeleeEnemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AFrostBall::AFrostBall()
{
	WeaponName = "Frost Ball";
	WeaponDescription = "A Frost Ball";
	AttackSpeed = 2000;
	Damage = 0;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh"));
	BallMesh->SetupAttachment(RootComponent);

}

void AFrostBall::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	if (ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(Character))
	{
		Player->DropWeapon();
	}
	FVector LaunchDirection = Character->GetActorForwardVector();
	LaunchDirection.Normalize();
	LaunchDirection *= 3;
	BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	BallMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetPhysicsLinearVelocity(LaunchDirection * AttackSpeed);
	BallMesh->SetEnableGravity(true);
	BallMesh->SetNotifyRigidBodyCollision(true);
	BallMesh->OnComponentHit.AddDynamic(this, &AFrostBall::OnHit);
}

void AFrostBall::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentBeginOverlap.AddDynamic(this, &AFrostBall::OnOverlap);
}

void AFrostBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFrostBall::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
}

void AFrostBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, BallMesh->GetComponentLocation(), FRotator::ZeroRotator, FVector(3, 3, 3));
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
