// Fill out your copyright notice in the Description page of Project Settings.


#include "Axe.h"

UAxe::UAxe()
{
	WeaponName = "Axe";
	WeaponDescription = "An Axe";
}

void UAxe::UseWeapon(ACharacter* Character)
{
	Super::UseWeapon(Character);
	
	TArray<FHitResult> HitResults;
	const FVector Start = Character->GetActorLocation();
	const FVector End = Start + Character->GetActorForwardVector() * AttackDistance;
	const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(AttackDistance));
	const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, End, End, Character->GetActorQuat(), ECC_WorldDynamic, CubeShape);

	TArray<AActor*> HitThisPunch;
	for(FHitResult HitResult : HitResults)
	{
		if (HitResult.GetActor() != Character && !HitThisPunch.Contains(HitResult.GetActor()))
		{
			HitThisPunch.Add(HitResult.GetActor());
			// AEnemy* HitEnemy = Cast<AEnemy>(HitResult.AEnemyGetActor());
			// if (HitEnemy)
			// {
			// 	HitEnemy->Ragdoll();
			// 	FVector LaunchDirection = HitEnemy->GetActorLocation() - Character->GetActorLocation();
			// 	LaunchDirection.Normalize();
			// 	LaunchDirection *= 3;
			// 	LaunchDirection += FVector::UpVector;
			// 	HitEnemy->GetMesh()->AddImpulse(LaunchDirection * PunchForce);
			// 	HitEnemy->DealDamage(PunchDamage);
			// }
		}
	}
}
