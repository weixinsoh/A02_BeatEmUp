// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "NiagaraFunctionLibrary.h"
#include "CoreMinimal.h"
#include "InGameUI.h"
#include "InventoryWidget.h"
#include "PauseMenu.h"
#include "Portal.h"
#include "Weapon.h"
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
	
	// Punch
	UPROPERTY(EditAnywhere)
		UInputAction* PunchAction;

	UPROPERTY(EditAnywhere)
		float PunchDistance = 250;

	UPROPERTY(EditAnywhere)
		float PunchForce = 50000;

	UPROPERTY(EditAnywhere)
		float PunchDamage = 10;

	UPROPERTY(EditAnywhere)
		float PunchCooldown = 1;

	bool bPunchReady = true;
	FTimerHandle PunchTimerHandle;
	
	UFUNCTION()
		void Punch();

	UFUNCTION()
		void ResetPunch();

	void DealDamage(float Damage);

	// Interact with interactable actor
	UPROPERTY(EditAnywhere)
		UInputAction* UseAction;

	UPROPERTY(EditAnywhere)
		float UseDistance = 1000;

	UFUNCTION()
		void Use();

	// Grab and throw enemy
	UPROPERTY(EditAnywhere)
		UInputAction* GrabAction;

	UPROPERTY(EditAnywhere)
		float GrabDistance = 250;

	UPROPERTY(EditAnywhere)
		float ThrowForce = 50000;

	bool IsGrabbingEnemy;

	UFUNCTION()
		void Grab();

	UFUNCTION()
		void Throw();

	UPROPERTY(EditAnywhere)
		AEnemy* GrabbedEnemy;

	// In game UI
	UPROPERTY(EditAnywhere)
		TSubclassOf<UInGameUI> InGameUIClass;

	UPROPERTY()
		UInGameUI* InGameUI;

	// Inventory system and weapon
	UPROPERTY(EditAnywhere)
		TArray<AWeapon*> Inventory;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere)
		UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere)
		int32 InventorySize = 4;
	
	UPROPERTY(EditAnywhere)
		UInputAction* DropAction;

	UPROPERTY(EditAnywhere)
		UInputAction* UseWeaponAction;

	UPROPERTY(EditAnywhere)
		UInputAction* ShowCursorAction;

	UPROPERTY(EditAnywhere)
		AWeapon* EquippingWeapon;

	UFUNCTION()
		void PickUp(AWeapon* Weapon);

	UFUNCTION()
		void DropWeapon();

	UFUNCTION()
		AWeapon* GetWeaponAtIndex(int32 Index);

	UFUNCTION()
		void UseWeapon();

	UFUNCTION()
		void ShowCursor();

	// To Boss World
	UPROPERTY(EditAnywhere)
		int NumEnemiesToDefeat = 1;

	UPROPERTY(EditAnywhere)
		int NumEnemiesDefeated;

	UPROPERTY(EditAnywhere)
		TSubclassOf<APortal> PortalClass;

	bool bPortalIsSpawned;

	// Adding progression
	UPROPERTY(EditAnywhere)
		int CurrentEXP = 0;
	
	UPROPERTY(EditAnywhere)
		int EXPToLevel = 50;
	
	UPROPERTY(EditAnywhere)
		float IncreaseMultiplier = 1.5;

	void AddEXP(int EXPToAdd);

	// Saving and Loading
	UPROPERTY(EditAnywhere)
		UInputAction* PauseAction;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UPauseMenu> PauseMenuClass;

	UFUNCTION()
		void PauseGame();

	UPROPERTY(EditAnywhere)
		UNiagaraSystem* PunchEffectClass;

	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};

