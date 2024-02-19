// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Interfaces/InteractionInterface.h"
#include "LooterShooterCharacter.generated.h"

class UTimelineComponent;
class UItemBase;
class ALooterHUD;
class UInventoryComponent;
class UQuestComponent;
class UHealthComponent;
class UWeaponComponent;
class UParkourMovementComponent;

USTRUCT()
struct FInteractionData
{
	GENERATED_USTRUCT_BODY()

	FInteractionData() : CurrentInteractable(nullptr), LastInteractionCheckTime(0.0f)
	{

	};

	UPROPERTY()
	AActor* CurrentInteractable;

	UPROPERTY()
	float LastInteractionCheckTime;
};

UCLASS(config=Game)
class ALooterShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bAiming;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	ALooterShooterCharacter(const FObjectInitializer& ObjInit);
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return SpringArmComponent; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }
	FORCEINLINE bool IsAimming() const { return bAiming; }

	FORCEINLINE UInventoryComponent* GetInventory() const { return PlayerInventory; }
	FORCEINLINE UQuestComponent* GetQuestComponent() const { return PlayerQuestComponent; }
	FORCEINLINE ALooterHUD* GetHUD() const { return HUD; }
	FORCEINLINE UTimelineComponent* GetTimeline() { return Timeline; }
	FORCEINLINE UCurveFloat* GetTimelineCurve() { return TimelineCurve; }


	void UpdateInteractionWidget() const;

	void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop);

	UFUNCTION(BlueprintCallable)
	bool CanSprinting() const;

	UParkourMovementComponent* GetParkourMovementComponent() const;

	bool GetCharacterIsAnimatingNow() const;

protected:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY()
	ALooterHUD* HUD;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComponent;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* NextWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SitDownAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleInventoryAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ToggleQuestAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AimingAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
	TScriptInterface<IInteractionInterface> TargetInteractable;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, Category = "Character | Components")
	UInventoryComponent* PlayerInventory;

	UPROPERTY(VisibleAnywhere, Category = "Character | Components")
	UQuestComponent* PlayerQuestComponent;

	float InteractionCheckFrequency;
	float InteractionCheckDistance;
	FTimerHandle TimerHandle_Interaction;
	FInteractionData InteractionData;

	UPROPERTY(VisibleAnywhere, Category = "Caharacter | Camera")
	FVector DefaultCameraLocation;

	UPROPERTY(VisibleAnywhere, Category = "Caharacter | Camera")
	FVector AimingCameraLocation;

	UTimelineComponent* Timeline;

	UPROPERTY(EditDefaultsOnly, Category = "Caharacter | Aim Timeline")
	UCurveFloat* TimelineCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Caharacter | Landed")
	FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

	UPROPERTY(EditDefaultsOnly, Category = "Caharacter | Landed")
	FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	void ToggleInventory();
	void ToggleQuestLog();

	void Aim();
	void StopAiming();
	UFUNCTION()
	void UpdateCameraTimeline(const float TimelineValue) const;
	UFUNCTION()
	void TimelineEnd();

	void PerformInteractionCheck();
	void FoundInteractable(AActor* NewInteractable);
	void NoInteractableFound();
	void BeginInteract();
	void EndInteract();
	void Interact();
	//void ObjectiveIDCalled(FName ObjectiveID);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

private:
	bool IsMovingForward = false;

	void OnDeath();
	void OnHealthChanged(float Health);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit);
};

