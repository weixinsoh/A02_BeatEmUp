// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "BrainComponent.h"
#include "BossBTController.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

ABossEnemy::ABossEnemy()
{
}

void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	TargetPlayer = GetWorld()->GetFirstPlayerController()->GetPawn();

	BossUI = Cast<UBossUI>(CreateWidget(GetGameInstance(), BossUIClass));
	MaxHealth = 1000;
	CurrentHealth = MaxHealth;
	if (BossUI)
	{
		BossUI->BossEnemy = this;
		BossUI->UpdateValues();
		BossUI->AddToViewport();
	}
}

void ABossEnemy::Ragdoll()
{
	if (ABossBTController* BossBTController = Cast<ABossBTController>(GetController()))
	{
		BossBTController->BrainComponent->PauseLogic("Ragdolling!");
	}
	Super::Ragdoll();
}

void ABossEnemy::StopRagdoll()
{
	if (ABossBTController* BossBTController = Cast<ABossBTController>(GetController()))
	{
		BossBTController->BrainComponent->ResumeLogic("Moving Again!");
	}
	Super::StopRagdoll();
}

void ABossEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
                       const FHitResult& Hit)
{
}

void ABossEnemy::DealDamage(float Damage)
{
	Super::DealDamage(Damage);
	BossUI->UpdateValues();
}

void ABossEnemy::SpawnEnemy()
{
	FVector LeftLocation = GetActorLocation() + GetActorForwardVector() * 100 - GetActorRightVector() * 100;
	FVector RightLocation = GetActorLocation() + GetActorForwardVector() * 100 + GetActorRightVector() * 100;
	FRotator Rotation = GetActorRotation();
	ABossBTController* BossBTController = Cast<ABossBTController>(GetController());
	BossBTController->LeftEnemyChild = GetWorld()->SpawnActor(EnemyClass, &LeftLocation, &Rotation);
	BossBTController->RightEnemyChild = GetWorld()->SpawnActor(EnemyClass, &RightLocation, &Rotation);
	BossBTController->bIsLeftChildDefeated = false;
	BossBTController->bIsRightChildDefeated = false;
}

void ABossEnemy::Attack()
{
	FVector MiddleLocation = GetActorLocation() + FVector::UpVector * 150;
	FVector LeftLocation = GetActorLocation() + FVector::UpVector * 150 - GetActorRightVector() * 100;
	FVector RightLocation = GetActorLocation() + FVector::UpVector * 150 + GetActorRightVector() * 100;
	FRotator Rotation = GetActorRotation();
	AStick* MiddleStick = Cast<AStick>(GetWorld()->SpawnActor(StickClass, &MiddleLocation, &Rotation));
	AStick* LeftStick = Cast<AStick>(GetWorld()->SpawnActor(StickClass, &LeftLocation, &Rotation));
	AStick* RightStick = Cast<AStick>(GetWorld()->SpawnActor(StickClass, &RightLocation, &Rotation));
	LeftStick->ProjectTime += 1;
	RightStick->ProjectTime += 2;
	MiddleStick->DelayProject();
	LeftStick->DelayProject();
	RightStick->DelayProject();
}
