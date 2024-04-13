// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "BeatEmUpCharacter.generated.h"

class AEnemy;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABeatEmUpCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	ABeatEmUpCharacter();

	UPROPERTY(EditAnywhere)
		int MaxHealth = 100;

	UPROPERTY(EditAnywhere)
		int CurrentHealth = MaxHealth;

	UPROPERTY(EditAnywhere)
		UInputAction* PunchAction;

	UPROPERTY(EditAnywhere)
		float PunchDistance = 250;

	UPROPERTY(EditAnywhere)
		float PunchForce = 50000;

	UPROPERTY(EditAnywhere)
		float PunchDamage = 50;

	UPROPERTY(EditAnywhere)
		float PunchCooldown = 1;

	bool bPunchReady = true;
	FTimerHandle PunchTimerHandle;

	UFUNCTION()
		void Punch();

	UFUNCTION()
		void ResetPunch();

	void DealDamage(float Damage);

	UPROPERTY(EditAnywhere)
		UInputAction* UseAction;

	UPROPERTY(EditAnywhere)
		float UseDistance = 1000;

	UFUNCTION()
		void Use();

	UPROPERTY(EditAnywhere)
		UInputAction* GrabAction;

	UPROPERTY(EditAnywhere)
		float GrabDistance = 250;
	
	UPROPERTY(EditAnywhere)
		float ThrowForce = 50000;

	UFUNCTION()
		void Grab();

	UFUNCTION()
		void Throw();

	UPROPERTY(EditAnywhere)
		AEnemy* GrabbedEnemy;
	bool IsGrabbingObject;
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};

