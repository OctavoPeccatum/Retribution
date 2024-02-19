// Copyright Epic Games, Inc. All Rights Reserved.

#include "LooterShooterCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/InputComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/QuestComponent.h"
#include "Items/ItemBase.h"
#include "Items/Pickup.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"
#include "UserInterfaces/LooterHUD.h"
#include "Components/ParkourMovementComponent.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(CharacterLog, All, All);

//////////////////////////////////////////////////////////////////////////
// ALooterShooterCharacter

ALooterShooterCharacter::ALooterShooterCharacter(const FObjectInitializer& ObjInit) 
	: Super(ObjInit.SetDefaultSubobjectClass<UParkourMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(30.0f, 90.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 150.0f; // The camera follows at this distance behind the character	
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	
	PlayerInventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));
	PlayerInventory->SetSlotCapacity(20);
	PlayerInventory->SetWeightCapacity(50.0f);

	PlayerQuestComponent = CreateDefaultSubobject<UQuestComponent>(TEXT("PlayerQuestComponent"));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	Timeline->SetPlayRate(1.0f);
	Timeline->SetTimelineLength(5.0f);
	DefaultCameraLocation = FVector{ 0.0f, 60.0f, 0.0f };
	AimingCameraLocation = FVector{ 105.0f, 30.0f, 0.0f };
	SpringArmComponent->SocketOffset = DefaultCameraLocation;

	InteractionCheckFrequency = 0.1f;
	InteractionCheckDistance = 450.0f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ALooterShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	check(SpringArmComponent);
	check(HealthComponent);
	check(GetParkourMovementComponent());
	check(WeaponComponent);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ALooterShooterCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ALooterShooterCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ALooterShooterCharacter::OnGroundLanded);

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	HUD = Cast<ALooterHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	FOnTimelineFloat LerpAlphaValue;
	FOnTimelineEvent TimelineFinishEvent;
	LerpAlphaValue.BindUFunction(this, FName("UpdateCameraTimeline"));
	TimelineFinishEvent.BindUFunction(this, FName("TimelineEnd"));

	if (Timeline && TimelineCurve)
	{
		Timeline->AddInterpFloat(TimelineCurve, LerpAlphaValue);
		Timeline->SetTimelineFinishedFunc(TimelineFinishEvent);
	}
}

void ALooterShooterCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetWorld()->TimeSince(InteractionData.LastInteractionCheckTime) > InteractionCheckFrequency)
	{
		PerformInteractionCheck();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALooterShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, GetParkourMovementComponent(), &UParkourMovementComponent::OnOvercoming);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ALooterShooterCharacter::BeginInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ALooterShooterCharacter::EndInteract);

		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, WeaponComponent, &UWeaponComponent::StartFire);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, WeaponComponent, &UWeaponComponent::StopFire);

		EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Started, WeaponComponent, &UWeaponComponent::NextWeapon);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, WeaponComponent, &UWeaponComponent::Reload);

		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &ALooterShooterCharacter::Aim);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &ALooterShooterCharacter::StopAiming);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, GetParkourMovementComponent(), &UParkourMovementComponent::OnStartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, GetParkourMovementComponent(), &UParkourMovementComponent::OnOngoingSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, GetParkourMovementComponent(), &UParkourMovementComponent::OnStopSprint);

		EnhancedInputComponent->BindAction(SitDownAction, ETriggerEvent::Started, GetParkourMovementComponent(), &UParkourMovementComponent::OnSitDown);

		EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ALooterShooterCharacter::ToggleInventory);

		EnhancedInputComponent->BindAction(ToggleQuestAction, ETriggerEvent::Started, this, &ALooterShooterCharacter::ToggleQuestLog);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALooterShooterCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALooterShooterCharacter::Look);

	}

}

void ALooterShooterCharacter::PerformInteractionCheck()
{
	InteractionData.LastInteractionCheckTime = GetWorld()->GetTimeSeconds();
	
	FVector TraceStart{FollowCamera->GetComponentLocation()};
	FVector TraceEnd{ TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};
	
		//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		FHitResult TraceHit;

		if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
			{
				if (TraceHit.GetActor() != InteractionData.CurrentInteractable)
				{
					FoundInteractable(TraceHit.GetActor());
					return;
				}

				if (TraceHit.GetActor() == InteractionData.CurrentInteractable) return;
				
			}
		}

	NoInteractableFound();
}

void ALooterShooterCharacter::FoundInteractable(AActor* NewInteractable)
{
	if (IsInteracting())
	{
		EndInteract();
	}

	if (InteractionData.CurrentInteractable)
	{
		TargetInteractable = InteractionData.CurrentInteractable;
		TargetInteractable->EndFocus();
	}

	InteractionData.CurrentInteractable = NewInteractable;
	TargetInteractable = NewInteractable;

	HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

	TargetInteractable->BeginFocus();
}

void ALooterShooterCharacter::NoInteractableFound()
{
	if (IsInteracting())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
	}

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->EndFocus();
		}

		HUD->HideInteractionWidget();

		InteractionData.CurrentInteractable = nullptr;
		TargetInteractable = nullptr;
	}
}

void ALooterShooterCharacter::BeginInteract()
{
	//Nothing state with for begining interaction
	PerformInteractionCheck();

	if (InteractionData.CurrentInteractable)
	{
		if (IsValid(TargetInteractable.GetObject()))
		{
			TargetInteractable->BeginInteract();

			if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
			{
				Interact();
			}
			else
			{
				GetWorldTimerManager().SetTimer(TimerHandle_Interaction,
					this,
					&ALooterShooterCharacter::Interact,
					TargetInteractable->InteractableData.InteractionDuration,
					false);
			}
		}
	}
}

void ALooterShooterCharacter::EndInteract()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		TargetInteractable->EndInteract();
	}
}

void ALooterShooterCharacter::Interact()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);

	if (IsValid(TargetInteractable.GetObject()))
	{
		FName ObjectiveID = TargetInteractable->Interact(this);
	}
}

void ALooterShooterCharacter::UpdateInteractionWidget() const
{
	if (IsValid(TargetInteractable.GetObject()))
	{
		HUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
	}
}

void ALooterShooterCharacter::ToggleInventory()
{
	HUD->ToggleInventory();

	if (HUD->bIsMenuVisible)
	{
		StopAiming();
	}
}

void ALooterShooterCharacter::ToggleQuestLog()
{
	HUD->ToggleQuestLog();

	if (HUD->bIsMenuVisible)
	{
		StopAiming();
	}
}

void ALooterShooterCharacter::Aim()
{
	if (!HUD->bIsMenuVisible)
	{
		bAiming = true;
		GetCharacterMovement()->MaxWalkSpeed = 200.0f;

		if (Timeline)
		{
			Timeline->SetTimelineLength(0.2f);
			Timeline->PlayFromStart();
		}

		InteractionCheckDistance = 300.0f;
	}
}

void ALooterShooterCharacter::StopAiming()
{
	if (bAiming)
	{
		bAiming = false;
		GetCharacterMovement()->MaxWalkSpeed = 350.0f;

		if (Timeline)
		{
			Timeline->SetTimelineLength(0.2f);
			Timeline->Reverse();
		}

		InteractionCheckDistance = 450.0f;
	}
}

void ALooterShooterCharacter::UpdateCameraTimeline(const float TimelineValue) const
{
	if (GetParkourMovementComponent()->GetMovementState() == EMovementState::Action)
	{
		GetParkourMovementComponent()->MantleUpdate(TimelineValue);
		WeaponComponent->StopFire();
	}
	else
	{
		const FVector CameraLocation = FMath::Lerp(DefaultCameraLocation, AimingCameraLocation, TimelineValue);
		SpringArmComponent->SocketOffset = CameraLocation;
	}	
}

void ALooterShooterCharacter::TimelineEnd()
{
		if (GetParkourMovementComponent()->GetMovementState() == EMovementState::Action)
		{
			GetParkourMovementComponent()->MantleEnd();
		}
}

void ALooterShooterCharacter::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
	if (PlayerInventory->FindMatchingItem(ItemToDrop))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.bNoFail = true;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const FVector SpawnLocation{ GetActorLocation() + (GetActorForwardVector() * 50.0f) };
		const FTransform SpawnTransform(GetActorRotation(), SpawnLocation);

		const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

		APickup* Pickup = GetWorld()->SpawnActor<APickup>(APickup::StaticClass(), SpawnTransform, SpawnParams);

		Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item to drop was somehow null!"));
	}
}

void ALooterShooterCharacter::Move(const FInputActionValue& Value)
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
		IsMovingForward = MovementVector.Y > 0.0f;
		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

	}
}

void ALooterShooterCharacter::Look(const FInputActionValue& Value)
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

void ALooterShooterCharacter::OnDeath()
{
	UE_LOG(CharacterLog, Display, TEXT("Player %s is dead"), *GetName());

	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(5.0f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();
}

void ALooterShooterCharacter::OnHealthChanged(float Health)
{

}

void ALooterShooterCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetVelocity().Z;

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(CharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

bool ALooterShooterCharacter::CanSprinting() const
{
	return IsMovingForward && !GetVelocity().IsZero() && !WeaponComponent->GetWeaponIsAnimatingNow();
}

UParkourMovementComponent* ALooterShooterCharacter::GetParkourMovementComponent() const
{
	const auto MovementComponent = Cast<UParkourMovementComponent>(GetCharacterMovement());
	return MovementComponent;
}

bool ALooterShooterCharacter::GetCharacterIsAnimatingNow() const
{
	return WeaponComponent->GetWeaponIsAnimatingNow() || GetParkourMovementComponent()->GetMovementState() == EMovementState::Action;
}








