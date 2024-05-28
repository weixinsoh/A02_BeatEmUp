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

	if (BulletTrailSystem)
	{
		BulletTrailComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BulletTrailSystem, Mesh->GetComponentLocation(), Mesh->GetComponentRotation());
	}

}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(BulletTrailComponent)
	{
		BulletTrailComponent->SetWorldLocation(Mesh->GetComponentLocation());
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
			BulletTrailComponent->DestroyComponent();
		}
		ABeatEmUpCharacter* HitCharacter = Cast<ABeatEmUpCharacter>(OtherActor);
		if (HitCharacter)
		{
			HitCharacter->DealDamage(Damage);
			Destroy();
			BulletTrailComponent->DestroyComponent();
		}
	}

}

