#pragma once

#include "LooterShooterCoreTypes.generated.h"

// Weapon
class ABaseWeapon;

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 ReservBullets;
};

// WeaponComponent

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	UTexture2D* BulletsIcon;

};


// HealthComponent

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

// MovementComponent
class UCurveVector;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSetNewLocation, FVector)

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Crouch		UMETA(DisplayName = "Crouch"),
	Running		UMETA(DisplayName = "Running"),
	Action		UMETA(DisplayName = "Action")
};

UENUM(BlueprintType)
enum class EMantleType : uint8
{
	HighMantle		UMETA(DisplayName = "Idle"),
	LowMantle		UMETA(DisplayName = "Crouch"),
	FallingCatch	UMETA(DisplayName = "Running")
};

USTRUCT(BlueprintType)
struct FTraceSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MaxLedgeHeight = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MinLedgeHeight = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ReachDistance = 75.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float ForwardTraceRadius = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float DonwardTraceRadius = 30.0f;
};

USTRUCT(BlueprintType)
struct FMantleAsset
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	UCurveVector* CorrectionCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	FVector StartingOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float LowHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float LowPlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float LowStartPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float HighHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float HighPlayRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float HighStartPostion;
};

USTRUCT(BlueprintType)
struct FMantleParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleParam")
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleParam")
	UCurveVector* CorrectionCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleParam")
	FVector StartingOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float StartingPosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleAsset")
	float PlayRate;
};

USTRUCT(BlueprintType)
struct FMantleLedgeLS
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleLedgeLS")
	FTransform MantleLedgeLSTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleLedgeLS")
	TWeakObjectPtr<UPrimitiveComponent> HitComponentLS;
};

USTRUCT(BlueprintType)
struct FMantleLedgeWS
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleLedgeWS")
	FTransform MantleLedgeWSTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "MantleLedgeWS")
	TWeakObjectPtr<UPrimitiveComponent> HitComponentWS;
};