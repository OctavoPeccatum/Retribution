// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LooterShooterCoreTypes.h"
#include "ParkourMovementComponent.generated.h"

class ALooterShooterCharacter;
/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UParkourMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	float RunModifier = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	float CrouchModifier = 2.0f;

	FOnSetNewLocation OnSetLocation;

	virtual float GetMaxSpeed() const override;

	UFUNCTION()
	void OnSitDown();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool GetIsCrouched() const { return bIsCrouched; }

	UFUNCTION(BlueprintCallable)
	EMovementState GetMovementState() const { return MovementState; }

	void OnStartSprint();
	void OnOngoingSprint();
	void OnStopSprint();
	void OnOvercoming();
	void MantleUpdate(const float TimelineValue);
	void MantleEnd();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* LowMantleAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HighMantleAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* VaultAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* RollAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "MantleTrace")
	FTraceSettings TraceSetings;

	UPROPERTY(EditDefaultsOnly, Category = "MantleTrace")
	float ZOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "MantleCurve")
	UCurveVector* CorrectionCurve1m;

	UPROPERTY(EditDefaultsOnly, Category = "MantleCurve")
	UCurveVector* CorrectionCurve2m;

	virtual void BeginPlay() override;

	void SetMovementState(EMovementState NewMovementState);

	void StartCrouch();
	void StopCrouch();

	void StartSprint();
	void StopSrint();

	bool MantleCheck();
	bool TraceForward(FVector& InitialTraceImpactPoint, FVector& InitialTraceNormal);
	bool TraceDownward(FVector InitialTraceImpactPoint, FVector InitialTraceNormal, FVector& DownTraceLocation, TWeakObjectPtr<UPrimitiveComponent>& HitComponent);
	void TraceInTheDistance(FVector InitialTraceImpactPoint, FVector InitialTraceNormal, FVector& DownTraceLocationInTheDistance);
	bool CapsuleHasRoomToStand(FTransform& TargetTransform, float& MantleHeight, FVector DownTraceLocation, FVector InitialTraceNormal);
	bool CapsuleHasRoomToCheck(FVector DownTraceLocation, UCapsuleComponent* Capsule);
	FVector GetCapsuleLocationFromBase(FVector DownTraceLocation);
	FVector GetCapsuleBaseLocation();
	void MantleStart(float MantleHeight, FTransform MantleLedgeWSTransform, TWeakObjectPtr<UPrimitiveComponent> MantleLedgeWSComponent, EMantleType MantleType);

private:
	EMovementState MovementState = EMovementState::Idle;
	EMovementState LastMovementState = EMovementState::Idle;
	bool bIsCrouched = false;
	float TraceRange = 50.0f;
	float TraceHeight = 200.0f;
	FMantleParams MantleParams;
	FMantleLedgeLS MantleLedgeLS;
	FTransform MantleAnimatedStartOffset, MantleActualStartOffset;

	ALooterShooterCharacter* GetCharacter() const;
	EMantleType	DetermineMantleType(float MantleHeight);
	FMantleAsset DetermineMantleAsset(EMantleType MantleType);
	FMantleParams DetermineMantleParams(EMantleType MantleType, float MantleHeight);
	FTransform CalculateStarttingOffset(TWeakObjectPtr<UPrimitiveComponent> HitComponent, FTransform MantleLedgeWSTransform);
	FTransform CalculateAnimatedStartOffset(FTransform MantleLedgeWSTransform);
	FTransform TransformMinusTransform(FTransform TransformA, FTransform TransformB);
	FTransform ComponentLocalToWorld();
	void ClearAndSetMantleState();
	void ConfiurateMantleTimeline();
	void PlayAnimMontageIfValid();
	void UpdatePositionAndCorrectionAlpha(float& PositionAlpha, float& XYCorrectionAlpha, float& ZCorrectionAlpha);
	FTransform CalculateLarpedTarget(FTransform MantleTargetTransform, float PositionAlpha, float XYCorrectionAlpha, float ZCorrectionAlpha, const float TimelineValue);
	FTransform BlendIntoTheAnimatedHorizontal(float XYCorrectionAlpha) const;
	FTransform BlendIntoTheAnimatedVertical(float ZCorrectionAlpha) const;
	FTransform TransformPlusTransform(FTransform TransformA, FTransform TransformB);
	void DoRoll();
	void InitAnimations();
	void OnActionStart(USkeletalMeshComponent* MeshComp, bool bNeedCollisionDisable);
	void OnActionEnd(USkeletalMeshComponent* MeshComp, bool bNeedCollisionDisable);
};
