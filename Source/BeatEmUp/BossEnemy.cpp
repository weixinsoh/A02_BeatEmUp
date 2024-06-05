// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"
#include "BrainComponent.h"
#include "BossBTController.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

ABossEnemy::ABossEnemy()
{
}

/**
 * This function is called when game starts to set the target player and create the health bar widget. 
 */
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

/**
 * This function is used to ragdoll the boss enemy before adding impulse on it. 
 */
void ABossEnemy::Ragdoll()
{
	if (ABossBTController* BossBTController = Cast<ABossBTController>(GetController()))
	{
		BossBTController->BrainComponent->PauseLogic("Ragdolling!");
	}
	Super::Ragdoll();
}

/**
 * This function is used to stop the ragdolling. 
 */
void ABossEnemy::StopRagdoll()
{
	if (ABossBTController* BossBTController = Cast<ABossBTController>(GetController()))
	{
		BossBTController->BrainComponent->ResumeLogic("Moving Again!");
	}
	Super::StopRagdoll();
}

/**
 * This function is overrides the onhit() method to prevent boss enemy from destroying itself. 
 */
void ABossEnemy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal,
                       const FHitResult& Hit)
{
}

/**
 * This function is used to deal damage to the boss enemy and update its health bar. 
 * @param Damage the damage value dealt to the boss enemy
 */
void ABossEnemy::DealDamage(float Damage)
{
	Super::DealDamage(Damage);
	BossUI->UpdateValues();
}

/**
 * This function is used to spawn two child enemies on its both sides. 
 */
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
	AEnemy* LeftEnemyChild = Cast<AEnemy>(BossBTController->LeftEnemyChild);
	LeftEnemyChild->bIsBossChild = true;
	AEnemy* RightEnemyChild = Cast<AEnemy>(BossBTController->RightEnemyChild);
	RightEnemyChild->bIsBossChild = true;
}

/**
 * This function is used to launched the sticks to attack the target player. 
 */
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
