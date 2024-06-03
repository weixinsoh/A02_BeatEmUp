// Fill out your copyright notice in the Description page of Project Settings.


#include "Stick.h"

#include "Kismet/KismetMathLibrary.h"

// Sets default values
AStick::AStick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	
	MovementComponent->bSweepCollision = true;
	MovementComponent->InitialSpeed = 1000;
	MovementComponent->UpdatedComponent = Mesh;
	MovementComponent->ProjectileGravityScale = 0;
	MovementComponent->SetActive(false);
}

void AStick::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
	const FHitResult& Hit)
{
	MovementComponent->SetActive(false);
	bDestroy = true;
	TArray<FHitResult> HitResults;
	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * 500;
	const FCollisionShape SphereShape = FCollisionShape::MakeSphere(500);
	const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, Start, End, GetActorQuat(), ECC_Pawn, SphereShape);

	TArray<AActor*> HitThisPunch;
	for(FHitResult HitResult : HitResults)
	{
		if (HitResult.GetActor() != this && !HitThisPunch.Contains(HitResult.GetActor()))
		{
			HitThisPunch.Add(HitResult.GetActor());
			ABeatEmUpCharacter* Player = Cast<ABeatEmUpCharacter>(HitResult.GetActor());
			if (Player)
			{
				Player->DealDamage(10);
			}
		}
	}
}

void AStick::DelayProject()
{
	GetWorld()->GetTimerManager().SetTimer(ProjectTimerHandle, this, &AStick::Project, ProjectTime, false); 
}

void AStick::Project()
{
	FVector TargetLocation = TargetPlayer->GetActorLocation();
	TargetLocation.Z = 0;
	FVector Direction = TargetLocation - Mesh->GetComponentLocation();
	Direction.Normalize();
	FRotator Rotation = Direction.Rotation();
	Rotation.Pitch = Rotation.Pitch - 270;
	SetActorRotation(Rotation);
	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
	MovementComponent->SetActive(true);
}

// Called when the game starts or when spawned
void AStick::BeginPlay()
{
	Super::BeginPlay();

	if (MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		Mesh->SetMaterial(0, MaterialInstance);
	}
	
	TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();
	Mesh->OnComponentHit.AddDynamic(this, &AStick::OnHit);
	Mesh->SetNotifyRigidBodyCollision(true);
	
	FRotator Rotation = GetActorRotation();
	Rotation.Pitch = Rotation.Pitch - 270;
	SetActorRotation(Rotation);
}

// Called every frame
void AStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bDestroy)
	{
		CurrentOpacity = FMath::Lerp(CurrentOpacity, 0, 3 * DeltaTime);
		MaterialInstance->SetScalarParameterValue("Opacity", CurrentOpacity);
		if (CurrentOpacity <= 0)
		{
			Destroy();
		}
	}
}

