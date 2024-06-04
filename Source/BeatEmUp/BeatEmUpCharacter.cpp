// Copyright Epic Games, Inc. All Rights Reserved.

#include "BeatEmUpCharacter.h"
#include "NiagaraComponent.h"
#include "Interactable.h"
#include "Enemy.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ABeatEmUpCharacter

ABeatEmUpCharacter::ABeatEmUpCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// Initialise the boolean of grabbing an enemy
	IsGrabbingEnemy = false;

	// Initialise the selected index in the inventory
	SelectedIndex = -1;

	// Initialise the number of enemies defeated as 0
	NumEnemiesDefeated = 0;

	bPortalIsSpawned = false;
	
}

void ABeatEmUpCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);
		InventoryWidget->Owner = this;
		InventoryWidget->AddToViewport();
	}
	
	InGameUI = Cast<UInGameUI>(CreateWidget(GetGameInstance(), InGameUIClass));
	if (InGameUI)
	{
		InGameUI->Player = this;
		InGameUI->UpdateValues();
		InGameUI->AddToViewport();
	}
}

void ABeatEmUpCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update the position of the enemy being grabbed to always locate in front of the player
	if (IsGrabbingEnemy)
	{
		FVector NewLocation = GetActorLocation() + GetActorForwardVector() * GrabDistance;
		NewLocation += FVector:: UpVector * 100;
		GrabbedEnemy->SetActorLocation(NewLocation);
	}

	if (GetWorld()->GetTimerManager().TimerExists(PunchTimerHandle))
	{
		InGameUI->UpdateValues();
	}
	FString LevelName = GetWorld()->GetMapName();
	LevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
	if (!bPortalIsSpawned && NumEnemiesDefeated >= NumEnemiesToDefeat && !LevelName.Equals("BossWorld"))
	{
		FVector SpawnLocation = FVector(GetActorLocation() + GetActorForwardVector() * 500);
		SpawnLocation.Z = 5;
		FRotator SpawnRotation = GetActorRotation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor(PortalClass, &SpawnLocation, &SpawnRotation, SpawnParams);
		bPortalIsSpawned = true;
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void ABeatEmUpCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABeatEmUpCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABeatEmUpCharacter::Look);

		// Punching
		EnhancedInputComponent->BindAction(PunchAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::Punch);

		// Using Gravity Well
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::Use);

		// Grab and throw
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::Grab);
		EnhancedInputComponent->BindAction(GrabAction, ETriggerEvent::Completed, this, &ABeatEmUpCharacter::Throw);
		
		// Drop weapon
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &ABeatEmUpCharacter::DropWeapon);

		// Use Weapon
		EnhancedInputComponent->BindAction(UseWeaponAction, ETriggerEvent::Triggered, this, &ABeatEmUpCharacter::UseWeapon);

		// Select Weapon
		EnhancedInputComponent->BindAction(SelectWeaponAction, ETriggerEvent::Triggered, this, &ABeatEmUpCharacter::SelectWeapon);

		// Pause Game
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ABeatEmUpCharacter::PauseGame);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ABeatEmUpCharacter::Punch()
{
	if (bPunchReady)
	{
		if (PunchEffectClass)
		{
			UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PunchEffectClass, GetActorLocation(), GetActorRotation());
			SpawnedEffect->SetColorParameter(FName("User.SpawnColour"), FLinearColor::MakeRandomColor());
		}
		bPunchReady = false;
		GetWorld()->GetTimerManager().SetTimer(PunchTimerHandle, this, &ABeatEmUpCharacter::ResetPunch, PunchCooldown,
		                                       false);
		TArray<FHitResult> HitResults;
		const FVector Start = GetActorLocation();
		const FVector End = Start + GetActorForwardVector() * PunchDistance;
		const FCollisionShape CubeShape = FCollisionShape::MakeBox(FVector(PunchDistance));
		const bool bSweep = GetWorld()->SweepMultiByChannel(HitResults, End, End, GetActorQuat(), ECC_WorldDynamic, CubeShape);

		TArray<AActor*> HitThisPunch;
		for(FHitResult HitResult : HitResults)
		{
			if (HitResult.GetActor() != this && !HitThisPunch.Contains(HitResult.GetActor()))
			{
				HitThisPunch.Add(HitResult.GetActor());
				AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor());
				if (HitEnemy)
				{
					HitEnemy->Ragdoll();
					FVector LaunchDirection = HitEnemy->GetActorLocation() - GetActorLocation();
					LaunchDirection.Normalize();
					LaunchDirection *= 3;
					LaunchDirection += FVector::UpVector;
					HitEnemy->GetMesh()->AddImpulse(LaunchDirection * PunchForce);
					HitEnemy->DealDamage(PunchDamage);
				}
			}
		}
	}
}

void ABeatEmUpCharacter::ResetPunch()
{
	bPunchReady = true;
	InGameUI->UpdateValues();
}

void ABeatEmUpCharacter::DealDamage(float Damage)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0, MaxHealth);

	InGameUI->UpdateValues();
	if (CurrentHealth == 0)
	{
		Cast<APlayerController>(GetController())->RestartLevel();
	}
}

void ABeatEmUpCharacter::Use()
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetFollowCamera()->GetForwardVector() * UseDistance;

	FHitResult HitData;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	TraceParams.TraceTag = FName("Use Trace Tag");
	GetWorld()->DebugDrawTraceTag = TraceParams.TraceTag;

	bool bSweep = GetWorld()->LineTraceSingleByChannel(HitData, Start, End, ECC_Visibility, TraceParams);

	if (bSweep)
	{
		if (HitData.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("We hit: %s"), *HitData.GetActor()->GetName());
			if (HitData.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
			{
				IInteractable::Execute_Interact(HitData.GetActor());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor not interactable!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("What happened!?"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Didn't hit an actor!"));
	}
}

/**
 * This function is used to grab an enemy using line trace.
 */
void ABeatEmUpCharacter::Grab()
{
	// Check the Player is already grabbing an enemy
	if (!IsGrabbingEnemy)
	{
		// Raycasting
		FHitResult HitResult;
		const FVector Start = GetActorLocation();
		const FVector End = Start + GetActorForwardVector() * GrabDistance;
	
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		TraceParams.AddIgnoredActors(ActorsToIgnore);
		TraceParams.TraceTag = FName("Grab Trace Tag");
		GetWorld()->DebugDrawTraceTag = TraceParams.TraceTag;

		bool bSweep = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_PhysicsBody, TraceParams);
	
		if (bSweep)
		{
			if (HitResult.GetActor() != this)
			{
				AEnemy* HitEnemy = Cast<AEnemy>(HitResult.GetActor());
				if (HitEnemy)
				{
					// Set the hit enemy as grabbed enemy
					GrabbedEnemy = HitEnemy;

					// Disable the enemy's movement
					HitEnemy->GetCharacterMovement()->SetMovementMode(MOVE_None);
					
					IsGrabbingEnemy = true;
				}
			}
		}	
	}
}

/**
 * This function is used to throw the grabbed enemy using physics mechanics. 
 */
void ABeatEmUpCharacter::Throw()
{
	if (IsGrabbingEnemy)
	{
		// Calculate the direction of throwing
		FVector LaunchDirection = GetFollowCamera()->GetForwardVector();
		LaunchDirection.Normalize();
		LaunchDirection *= 3;
		LaunchDirection += FVector::UpVector;

		// Apply the throwing on grabbed enemy
		GrabbedEnemy->Ragdoll();
		GrabbedEnemy->GetMesh()->AddImpulse(LaunchDirection * ThrowForce);

		// Enable the enemy's movement
		GrabbedEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		IsGrabbingEnemy = false;
	}
}

/**
 * This function is used to pick up a weapon by adding it to the inventory array.
 * It refreshes the inventory widget after picking up the weapon.
 * @param Weapon The weapon to be picked up.
 */
void ABeatEmUpCharacter::PickUp(AWeapon* Weapon)
{
	if(Inventory.Num() < InventorySize && !Inventory.Contains(Weapon))
	{
		Inventory.Add(Weapon);
		InventoryWidget->RefreshInventory(Inventory);
		Weapon->SetActorHiddenInGame(true);
		Weapon->PickedUpCharacter = this;
	}
}

/**
 * Returns a weapon in the inventory for a given index.
 * @param Index the index position of the weapon in the inventory.
 * @return a weapon in the inventory.
 */
AWeapon* ABeatEmUpCharacter::GetWeaponAtIndex(int32 Index)
{
	if(Index < 0 || Index >= Inventory.Num())
	{
		return nullptr;
	}
	return Inventory[Index];

}

/**
 * This function is used to drop the equipping weapon.
 * It detaches the weapon from the player's weapon socket
 * and removes it from the inventory.
 */
void ABeatEmUpCharacter::DropWeapon()
{
	if (EquippingWeapon)
	{
		EquippingWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippingWeapon->PickedUpCharacter = nullptr;
		Inventory.Remove(EquippingWeapon);
		InventoryWidget->RefreshInventory(Inventory);
		EquippingWeapon = nullptr;
	}
}

/**
 * This function is used to perform the attack action using the equipping weapon.
 */
void ABeatEmUpCharacter::UseWeapon()
{
	if (EquippingWeapon)
	{
		EquippingWeapon->UseWeapon(this);
	}
}

void ABeatEmUpCharacter::SelectWeapon()
{
	SelectedIndex = (SelectedIndex + 1) % InventorySize;
	InventoryWidget->OnButtonWasClicked(SelectedIndex);
}

void ABeatEmUpCharacter::AddEXP(int EXPToAdd)
{
	CurrentEXP += EXPToAdd;
	while (CurrentEXP > EXPToLevel)
	{
		CurrentEXP -= EXPToLevel;
		EXPToLevel *= IncreaseMultiplier;
		PunchDamage *= IncreaseMultiplier;
		MaxHealth *= IncreaseMultiplier;
		CurrentHealth = MaxHealth;
	}
	InGameUI->UpdateValues();
}

void ABeatEmUpCharacter::PauseGame()
{
	if(!UGameplayStatics::IsGamePaused(GetWorld()))
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		UPauseMenu* PauseMenu = Cast<UPauseMenu>(CreateWidget(GetGameInstance(),PauseMenuClass));
		if(PauseMenu)
		{
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			PauseMenu->AddToViewport();
		}
	}
}

void ABeatEmUpCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABeatEmUpCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
