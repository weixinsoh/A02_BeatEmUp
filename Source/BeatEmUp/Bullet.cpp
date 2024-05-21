// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Enemy.h"
#include "NiagaraComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	BulletTrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Bullet Trail"));
	BulletTrailComponent->SetupAttachment(RootComponent);

	MovementComponent->bShouldBounce = true;
	MovementComponent->BounceVelocityStopSimulatingThreshold = MovementSpeed / 2;
	MovementComponent->bSweepCollision = true;
	MovementComponent->InitialSpeed = MovementSpeed;
	MovementComponent->UpdatedComponent = Mesh;
	MovementComponent->ProjectileGravityScale = 0;

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	Mesh->SetNotifyRigidBodyCollision(true);

	if(BulletTrailSystem)
	{
		Test = UNiagaraFunctionLibrary::SpawnSystemAttached(BulletTrailSystem, Mesh, FName("None"), FVector(0,0,0), FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, true);
	}

}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(Test)
	{
		//Test->SetWorldLocation(Mesh->GetComponentLocation());
		Test->SetVectorParameter(FName("User.Location"), Mesh->GetComponentLocation());
	}
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
                    const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		AEnemy* HitEnemy = Cast<AEnemy>(OtherActor);
		if (HitEnemy)
		{
			HitEnemy->DealDamage(Damage);
			Destroy();
		}
		ABeatEmUpCharacter* HitCharacter = Cast<ABeatEmUpCharacter>(OtherActor);
		if (HitCharacter)
		{
			HitCharacter->DealDamage(Damage);
			Destroy();
		}
	}
	if (BulletTrailComponent)
	{
		BulletTrailComponent->DestroyComponent();
	}
}

