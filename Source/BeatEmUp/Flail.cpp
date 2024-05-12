// Fill out your copyright notice in the Description page of Project Settings.


#include "Flail.h"

#include "Enemy.h"

/**
 * Constructor for flail. This weapon uses physics constraints.
 */
AFlail::AFlail()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set weapon info
	WeaponName = "Flail";
	WeaponDescription = "A medieval melee weapon consisting of a spiked ball attached to a chain. "
					 "When swung, the flexible chain allows for unpredictable and devastating attacks. ";
	Damage = 5;
	AttackDistance = 800;
	AttackSpeed = 100;

	// Create component
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
	Chain10 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chain10"));

	Hinge12 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge12"));
	Hinge23 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge23"));
	Hinge34 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge34"));
	Hinge45 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge45"));
	Hinge56 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge56"));
	Hinge67 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge67"));
	Hinge78 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge78"));
	Hinge89 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge89"));
	Hinge90 = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Hinge90"));
	HingeBall = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("HingeBall"));

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
	Chain10->SetupAttachment(RootComponent);
	Hinge12->SetupAttachment(RootComponent);
	Hinge23->SetupAttachment(RootComponent);
	Hinge34->SetupAttachment(RootComponent);
	Hinge45->SetupAttachment(RootComponent);
	Hinge56->SetupAttachment(RootComponent);
	Hinge67->SetupAttachment(RootComponent);
	Hinge78->SetupAttachment(RootComponent);
	Hinge89->SetupAttachment(RootComponent);
	Hinge90->SetupAttachment(RootComponent);
	HingeBall->SetupAttachment(RootComponent);
}

/**
 * Throw the flail ball and collide with the world static and dynamic component. 
 * @param Character The character who use this weapon.
 */
void AFlail::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);

	// Calculate the launch direction
	FVector LaunchDirection = Character->GetActorForwardVector();
	LaunchDirection.Normalize();
	LaunchDirection *= 3;
	LaunchDirection += FVector::UpVector;

	// Set collision settings
	BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	BallMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	// Launch the flail ball
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetEnableGravity(true);
	BallMesh->SetNotifyRigidBodyCollision(true);
	BallMesh->SetPhysicsLinearVelocity(LaunchDirection * HitForce);
}

/**
 * Begin play function which dynamically delegate OnOverlap() function
 * once the ball is overlapped with something.
 */
void AFlail::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentBeginOverlap.AddDynamic(this, &AFlail::OnOverlap);

}

/**
 * This function update the position of the flail staying beside the player if it is picked up. 
 */
void AFlail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PickedUpCharacter != nullptr)
	{
		FVector Distance = PickedUpCharacter->GetActorForwardVector() * 100 + FVector::LeftVector * 200;
		SetActorLocation(PickedUpCharacter->GetActorLocation() + Distance);
	}
}

/**
 * Deal damage to the hit enemy.
 */
void AFlail::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	if (OtherActor && OtherActor != PickedUpCharacter)
	{
		AEnemy* HitEnemy = Cast<AEnemy>(OtherActor);
		if (HitEnemy)
		{
			HitEnemy->DealDamage(Damage);
		}
	}
}
