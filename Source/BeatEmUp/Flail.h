// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Flail.generated.h"

/**
 * 
 */
UCLASS()
class BEATEMUP_API AFlail : public AWeapon
{
	GENERATED_BODY()

public:

	AFlail();
	
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BallMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain1;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain2;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain3;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain4;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain5;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain6;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain7;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain8;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain9;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Chain10;

	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge12;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge23;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge34;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge45;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge56;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge67;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge78;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge89;
	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* Hinge90;

	UPROPERTY(EditAnywhere)
		UPhysicsConstraintComponent* HingeBall;

	UPROPERTY(EditAnywhere)
		float HitForce = 50000;

	virtual void UseWeapon(ACharacter* Character) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector Normal, const FHitResult& Hit);
};
