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
	Damage = 25;
	AttackDistance = 500;
	AttackSpeed = 10000;

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
	HingeBall->SetupAttachment(BallMesh);
	Chain1->SetupAttachment(HingeBall);
	Hinge12->SetupAttachment(Chain1);
	Chain2->SetupAttachment(Hinge12);
	Hinge23->SetupAttachment(Chain2);
	Chain3->SetupAttachment(Hinge23);
	Hinge34->SetupAttachment(Chain3);
	Chain4->SetupAttachment(Hinge34);
	Hinge45->SetupAttachment(Chain4);
	Chain5->SetupAttachment(Hinge45);
	Hinge56->SetupAttachment(Chain5);
	Chain6->SetupAttachment(Hinge56);
	Hinge67->SetupAttachment(Chain6);
	Chain7->SetupAttachment(Hinge67);
	Hinge78->SetupAttachment(Chain7);
	Chain8->SetupAttachment(Hinge78);
	Hinge89->SetupAttachment(Chain8);
	Chain9->SetupAttachment(Hinge89);
	Hinge90->SetupAttachment(Chain9);
	Chain10->SetupAttachment(Hinge90);
}

/**
 * Throw the flail ball and collide with the world static and dynamic component. 
 * @param Character The character who use this weapon.
 */
void AFlail::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);

	BallMesh->SetWorldLocation(Character->GetActorLocation() + Character->GetActorForwardVector() * 100);
	// Calculate the launch direction
	FVector LaunchDirection = Character->GetActorForwardVector();
	LaunchDirection.Normalize();
	LaunchDirection *= 3;
	LaunchDirection += FVector::UpVector;

	// Set collision settings
	BallMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BallMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	// Launch the flail ball
	BallMesh->SetSimulatePhysics(true);
	BallMesh->SetEnableGravity(true);
	BallMesh->SetNotifyRigidBodyCollision(true);
	BallMesh->AddImpulse(LaunchDirection * AttackSpeed);
}

/**
 * Begin play function which dynamically delegate OnOverlap() function
 * once the ball is overlapped with something.
 */
void AFlail::BeginPlay()
{
	Super::BeginPlay();
	BallMesh->OnComponentBeginOverlap.AddDynamic(this, &AFlail::OnOverlap);
	BallMesh->OnComponentHit.AddDynamic(this, &AFlail::OnHit);
}

/**
 * This function update the position of the flail staying beside the player if it is picked up. 
 */
void AFlail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			HitEnemy->Ragdoll();
			FVector LaunchDirection = HitEnemy->GetActorLocation() - GetActorLocation();
			LaunchDirection.Normalize();
			LaunchDirection *= 3;
			LaunchDirection += FVector::UpVector;
			HitEnemy->GetMesh()->AddImpulse(LaunchDirection * HitForce);
			HitEnemy->DealDamage(Damage);
		}
	}
	
	// Set collision settings to overlap with pawn
	BallMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}
