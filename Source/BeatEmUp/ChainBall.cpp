// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainBall.h"

#include "Enemy.h"
#include "GameFramework/Character.h"

AChainBall::AChainBall()
{
	WeaponName = "Chain Ball";
	WeaponDescription = "A Chain Ball";
	Damage = 5;
	AttackDistance = 800;
	AttackSpeed = 100;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	
	Chain1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain1"));
	Chain2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain2"));
	Chain3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain3"));
	Chain4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain4"));
	Chain5 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain5"));
	Chain6 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain6"));
	Chain7 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain7"));
	Chain8 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain8"));
	Chain9 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain9"));

	Hinge12 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge12"));
	Hinge23 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge23"));
	Hinge34 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge34"));
	Hinge45 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge45"));
	Hinge56 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge56"));
	Hinge67 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge67"));
	Hinge78 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge78"));
	Hinge78 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge89"));
	//HingeBall = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("BallHinge"));

	BallMesh->SetupAttachment(RootComponent);
	Chain1->SetupAttachment(RootComponent);
	Chain2->SetupAttachment(RootComponent);
	Chain3->SetupAttachment(RootComponent);
	Chain4->SetupAttachment(RootComponent);
	Chain5->SetupAttachment(RootComponent);
	Chain6->SetupAttachment(RootComponent);
	Chain7->SetupAttachment(RootComponent);
	Chain8->SetupAttachment(RootComponent);
	Chain9->SetupAttachment(RootComponent);
	Hinge12->SetupAttachment(RootComponent);
	Hinge23->SetupAttachment(RootComponent);
	Hinge34->SetupAttachment(RootComponent);
	Hinge45->SetupAttachment(RootComponent);
	Hinge56->SetupAttachment(RootComponent);
	Hinge67->SetupAttachment(RootComponent);
	Hinge78->SetupAttachment(RootComponent);
	//Hinge89->SetupAttachment(RootComponent);
	//HingeBall->SetupAttachment(RootComponent);

}

void AChainBall::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Chain1->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));
	FVector LaunchDirection = Character->GetActorLocation();
	LaunchDirection.Normalize();
	LaunchDirection *= 3;
	LaunchDirection += FVector::UpVector;
	BallMesh->SetSimulatePhysics(true);
	BallMesh->AddImpulse(LaunchDirection * HitForce);
}

void AChainBall::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentBeginOverlap.AddDynamic(this, &AChainBall::OnOverlap);
}

void AChainBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChainBall::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComponent, OtherBodyIndex, bFromSweep, SweepResult);
}

void AChainBall::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	if (OtherActor && OtherActor != PickedUpCharacter)
	{
		AEnemy* HitEnemy = Cast<AEnemy>(OtherActor);
		if (HitEnemy)
		{
			HitEnemy->DealDamage(Damage);
			Destroy();
		}
	}
}
