// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ParkourMovementComponent.h"
#include "Engine/World.h"
#include "LooterShooter/LooterShooterCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/TimelineComponent.h"
#include "DrawDebugHelpers.h"
#include "Animations/AnimUtils.h"
#include "Animations/SetNewLocationToMantleAnimNotify.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Private/KismetTraceUtils.h"
#include "Curves/CurveVector.h"

#define COLLISION_CLIMBABLE ECC_GameTraceChannel1
DEFINE_LOG_CATEGORY_STATIC(MovementLog, All, All);

float UParkourMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    if (!GetCharacter()) return MaxSpeed;
   
    switch (MovementState)
    {
    case EMovementState::Crouch:
        return MaxSpeed / CrouchModifier;
        break;
    case EMovementState::Running:
        return MaxSpeed * RunModifier;
        break;
    default:
        return MaxSpeed;
        break;
    }
}

void UParkourMovementComponent::OnSitDown()
{
    switch (MovementState)
    {
    case EMovementState::Idle:
        StartCrouch();
        break;
    case EMovementState::Crouch:
        StopCrouch();
        break;
    case EMovementState::Running:
        StartCrouch();
        break;
    case EMovementState::Action:
        break;
    default:
        break;
    }
}

void UParkourMovementComponent::StartCrouch()
{
   SetMovementState(EMovementState::Crouch);
}

void UParkourMovementComponent::StopCrouch()
{
    SetMovementState(EMovementState::Idle);
}

void UParkourMovementComponent::OnStartSprint()
{
    StartSprint();
}

void UParkourMovementComponent::OnStopSprint()
{
    StopSrint();
}

void UParkourMovementComponent::OnOvercoming()
{
    MantleCheck();
}

void UParkourMovementComponent::StartSprint()
{
    SetMovementState(EMovementState::Running);
}

void UParkourMovementComponent::StopSrint()
{
    if (MovementState == EMovementState::Running)
    {
        SetMovementState(EMovementState::Idle);
    }
}

void UParkourMovementComponent::HeightFinder()
{
    if (!GetWorld() || !GetCharacter() || GetCharacter()->GetCharacterIsAnimatingNow()) return;

    FVector TraceStart, TraceEnd;
    FQuat TraceRotation;
    FCollisionShape CharacterCapsule;
    if (!GetTraceData(TraceStart, TraceEnd, CharacterCapsule)) return;

    FHitResult HitResult;
    MakeTrace(HitResult, TraceStart, TraceEnd, CharacterCapsule);

    if (HitResult.bBlockingHit)
    {
        float MantleHeight = HitResult.Distance;
        if (MantleHeight >= 10.0f && MantleHeight <= 100.0f)
        {
            PlayAnimMontage(HighMantleAnimMontage);
        }
        else if (MantleHeight > 100.0f && MantleHeight < 130.0f)
        {
            PlayAnimMontage(LowMantleAnimMontage);
        }
        else
        {

        }
    }
}

void UParkourMovementComponent::MantleCheck()
{
    if (!GetWorld()) return;

    FVector InitialTraceImpactPoint, InitialTraceNormal;
    if (!TraceForward(InitialTraceImpactPoint, InitialTraceNormal)) return;

    FVector DownTraceLocation;
    TWeakObjectPtr<UPrimitiveComponent> HitComponent;
    if(!TraceDownward(InitialTraceImpactPoint, InitialTraceNormal, DownTraceLocation, HitComponent)) return;

    FTransform TargetTransform;
    float MantleHeight;
    if (!CapsuleHasRoomToStand(TargetTransform, MantleHeight, DownTraceLocation, InitialTraceNormal)) return;

    EMantleType MantleType = DetermineMantleType(MantleHeight);
    MantleLedgeLS.MantleLedgeLSTransform = TargetTransform * HitComponent->GetComponentToWorld().Inverse();
    MantleLedgeLS.HitComponentLS = HitComponent;
    MantleStart(MantleHeight, TargetTransform, HitComponent, MantleType);
}

bool UParkourMovementComponent::TraceForward(FVector& InitialTraceImpactPoint, FVector& InitialTraceNormal)
{
    FVector TraceStart = FVector(GetCapsuleBaseLocation() + (GetCharacter()->GetLastMovementInputVector() * ( - 30.0f))) + FVector(0.0f, 0.0f, (TraceSetings.MaxLedgeHeight + TraceSetings.MinLedgeHeight) / 2);
    FVector TraceEnd = TraceStart + (GetCharacter()->GetLastMovementInputVector() * TraceSetings.ReachDistance);
    float HalfHeight = ((TraceSetings.MaxLedgeHeight + TraceSetings.MinLedgeHeight) / 2) + 1;

    FHitResult HitResult;
    FCollisionQueryParams CollisionQueryParam;
    CollisionQueryParam.AddIgnoredActor(GetCharacter());
    GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::COLLISION_CLIMBABLE, FCollisionShape::MakeCapsule(TraceSetings.ForwardTraceRadius, HalfHeight), CollisionQueryParam);
    
#if ENABLE_DRAW_DEBUG
    DrawDebugCapsuleTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceSetings.ForwardTraceRadius, HalfHeight, EDrawDebugTrace::ForDuration, HitResult.bBlockingHit, HitResult, FLinearColor::Red, FLinearColor::Green, 3.0f);
#endif

    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    InitialTraceImpactPoint = HitResult.ImpactPoint;
    InitialTraceNormal = HitResult.ImpactNormal;

    UE_LOG(MovementLog, Display, TEXT("%s"), (HitResult.bBlockingHit ? TEXT("True") : TEXT("False")));
    if (HitResult.bBlockingHit)
    {
        UE_LOG(MovementLog, Display, TEXT("%s"), *HitResult.GetActor()->GetName());
    }
   
    return HitResult.bBlockingHit && !IsWalkable(HitResult);
}

bool UParkourMovementComponent::TraceDownward(FVector InitialTraceImpactPoint, FVector InitialTraceNormal, FVector& DownTraceLocation, TWeakObjectPtr<UPrimitiveComponent>& HitComponent)
{
    FVector TraceEnd = (FVector(InitialTraceImpactPoint.X, InitialTraceImpactPoint.Y, GetCapsuleBaseLocation().Z) + (InitialTraceNormal * (-15.0f)));
    FVector TraceStart = TraceEnd + FVector(0.0f, 0.0f, TraceSetings.MaxLedgeHeight + TraceSetings.DonwardTraceRadius );

    FHitResult HitResult;
    FCollisionQueryParams CollisionQueryParam;
    CollisionQueryParam.AddIgnoredActor(GetCharacter());
    GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::COLLISION_CLIMBABLE, FCollisionShape::MakeSphere(TraceSetings.DonwardTraceRadius), CollisionQueryParam);

#if ENABLE_DRAW_DEBUG
    DrawDebugSphereTraceSingle(GetWorld(), TraceStart, TraceEnd, TraceSetings.ForwardTraceRadius, EDrawDebugTrace::ForDuration, HitResult.bBlockingHit, HitResult, FLinearColor::Black, FLinearColor::Blue, 3.0f);
#endif

    DownTraceLocation = FVector(HitResult.Location.X, HitResult.Location.Y, HitResult.ImpactPoint.Z);
    HitComponent = HitResult.GetComponent();

    return HitResult.bBlockingHit && IsWalkable(HitResult);
}

bool UParkourMovementComponent::CapsuleHasRoomToStand(FTransform& TargetTransform, float& MantleHeight, FVector DownTraceLocation, FVector InitialTraceNormal)
{
    if(!CapsuleHasRoomToCheck(DownTraceLocation, GetCharacter()->GetCapsuleComponent())) return false;

    TargetTransform = FTransform((InitialTraceNormal * FVector(-1.0f, -1.0f, 0.0f)).Rotation(), GetCapsuleLocationFromBase(DownTraceLocation), FVector(1.0f));
    MantleHeight = (TargetTransform.GetLocation() - GetCharacter()->GetActorLocation()).Z;
    return true;
}

bool UParkourMovementComponent::CapsuleHasRoomToCheck(FVector DownTraceLocation, UCapsuleComponent* Capsule)
{
    FVector TraceStart = GetCapsuleLocationFromBase(DownTraceLocation) + FVector(0.0f, 0.0f, Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere());
    FVector TraceEnd = GetCapsuleLocationFromBase(DownTraceLocation) - FVector(0.0f, 0.0f, Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere());

    FHitResult HitResult;
    FCollisionQueryParams CollisionQueryParam;
    CollisionQueryParam.AddIgnoredActor(GetCharacter());
    GetWorld()->SweepSingleByProfile(HitResult, TraceStart, TraceStart, FQuat::Identity, FName(GetCharacter()->GetFName()), FCollisionShape::MakeSphere(Capsule->GetScaledCapsuleRadius()), CollisionQueryParam);

#if ENABLE_DRAW_DEBUG
    DrawDebugSphereTraceSingle(GetWorld(), TraceStart, TraceEnd, Capsule->GetScaledCapsuleRadius(), EDrawDebugTrace::ForDuration, HitResult.bBlockingHit, HitResult, FLinearColor::Gray, FLinearColor::Yellow, 3.0f);
#endif

    return !HitResult.bBlockingHit;
}

FVector UParkourMovementComponent::GetCapsuleLocationFromBase(FVector DownTraceLocation)
{
    return (DownTraceLocation + FVector(0.0f, 0.0f, GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 2.0f));
}

FVector UParkourMovementComponent::GetCapsuleBaseLocation()
{
    FVector WorldLocation = GetCharacter()->GetCapsuleComponent()->GetComponentLocation();
    FVector UpVector = GetCharacter()->GetCapsuleComponent()->GetUpVector();
    float HalfHeight = GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
    return (WorldLocation - (UpVector * (HalfHeight + ZOffset)));
}

void UParkourMovementComponent::MantleStart(float MantleHeight, FTransform MantleLedgeWSTransform, TWeakObjectPtr<UPrimitiveComponent> MantleLedgeWSComponent, EMantleType MantleType)
{
    MantleActualStartOffset = CalculateStarttingOffset(MantleLedgeWSComponent, MantleLedgeWSTransform);
 
    MantleParams = DetermineMantleParams(MantleType, MantleHeight);

    MantleAnimatedStartOffset = CalculateAnimatedStartOffset(MantleLedgeWSTransform);

    ClearAndSetMantleState();
    ConfiurateMantleTimeline();
    PlayAnimMontageIfValid();
}

void UParkourMovementComponent::MantleUpdate(const float TimelineValue)
{
    FTransform MantleTarget = ComponentLocalToWorld();

    float PositionAlpha, XYCorrectionAlpha, ZCorrectionAlpha;
    UpdatePositionAndCorrectionAlpha(PositionAlpha, XYCorrectionAlpha, ZCorrectionAlpha);

    FTransform LerpedTarget = CalculateLarpedTarget(MantleTarget, PositionAlpha, XYCorrectionAlpha, ZCorrectionAlpha, TimelineValue);
    GetCharacter()->SetActorLocationAndRotation(LerpedTarget.GetLocation(), GetCharacter()->GetActorRotation());
}

void UParkourMovementComponent::MantleEnd()
{
    UE_LOG(MovementLog, Display, TEXT("Mantle End"));
    SetMovementMode(EMovementMode::MOVE_Walking);
    MovementState = EMovementState::Idle;
}

bool UParkourMovementComponent::GetTraceData(FVector& TraceStart, FVector& TraceEnd, FCollisionShape& CharacterCapsule) const
{
    if (!GetCharacter()) return false;
    
    CharacterCapsule = FCollisionShape::MakeCapsule(GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
    TraceEnd = FVector(GetCharacter()->GetActorLocation().X, GetCharacter()->GetActorLocation().Y, GetCharacter()->GetActorLocation().Z - GetCharacter()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.0f) +(GetCharacter()->GetActorForwardVector() * TraceRange);
    TraceStart = FVector(TraceEnd.X, TraceEnd.Y, TraceEnd.Z + TraceHeight);
    return true;
}

void UParkourMovementComponent::MakeTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd, FCollisionShape& CharacterCapsule)
{
    FCollisionQueryParams CollisionParam;
    CollisionParam.AddIgnoredActor(GetOwner());
    /*GetWorld()->SweepSingleByChannel(HitResult,
        TraceStart, 
        TraceEnd, 
        FQuat::Identity, 
        ECollisionChannel::ECC_Visibility, 
        CharacterCapsule, 
        CollisionParam, 
        FCollisionResponseParams::DefaultResponseParam);
    */
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParam);
    DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
}

void UParkourMovementComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    if(!GetCharacter()) return;
    GetCharacter()->PlayAnimMontage(Animation);
}

ALooterShooterCharacter* UParkourMovementComponent::GetCharacter() const
{
    ALooterShooterCharacter* Player = Cast<ALooterShooterCharacter>(GetPawnOwner());
    return Player;
}

EMantleType UParkourMovementComponent::DetermineMantleType(float MantleHeight)
{
    if (MantleHeight > 125.0f)
    {
        return EMantleType(EMantleType::HighMantle);
    }
    return EMantleType(EMantleType::LowMantle);
}

FMantleAsset UParkourMovementComponent::DetermineMantleAsset(EMantleType MantleType)
{
    FMantleAsset MantleAsset;

    switch (MantleType)
    {
    case EMantleType::HighMantle:
        MantleAsset.AnimMontage = HighMantleAnimMontage;
        MantleAsset.CorrectionCurve = CorrectionCurve1m;
        MantleAsset.StartingOffset = FVector(0.0f, 65.0f, 200.0f);
        MantleAsset.LowHeight = 50.0f;
        MantleAsset.LowPlayRate = 1.0f;
        MantleAsset.LowStartPosition = 0.5f;
        MantleAsset.HighHeight = 100.0f;
        MantleAsset.HighPlayRate = 1.0f;
        MantleAsset.HighStartPostion = 1.0f;
        break;
    case EMantleType::LowMantle:
        MantleAsset.AnimMontage = LowMantleAnimMontage;
        MantleAsset.CorrectionCurve = CorrectionCurve2m;
        MantleAsset.StartingOffset = FVector(0.0f, 65.0f, 200.0f);
        MantleAsset.LowHeight = 125.0f;
        MantleAsset.LowPlayRate = 1.2f;
        MantleAsset.LowStartPosition = 0.6f;
        MantleAsset.HighHeight = 200.0f;
        MantleAsset.HighPlayRate = 1.2f;
        MantleAsset.HighStartPostion = 0.0f;
        break;
    }
    return MantleAsset;
}

FMantleParams UParkourMovementComponent::DetermineMantleParams(EMantleType MantleType, float MantleHeight)
{
    FMantleAsset MantleAsset = DetermineMantleAsset(MantleType);
    MantleParams.AnimMontage = MantleAsset.AnimMontage;
    MantleParams.CorrectionCurve = MantleAsset.CorrectionCurve;
    MantleParams.StartingOffset = MantleAsset.StartingOffset;
    MantleParams.PlayRate = FMath::GetMappedRangeValueClamped(FVector2D(MantleAsset.LowHeight, MantleAsset.HighHeight), FVector2D(MantleAsset.LowPlayRate, MantleAsset.HighPlayRate), MantleHeight);
    MantleParams.StartingPosition = FMath::GetMappedRangeValueClamped(FVector2D(MantleAsset.LowHeight, MantleAsset.HighHeight), FVector2D(MantleAsset.LowStartPosition, MantleAsset.HighStartPostion), MantleHeight);
    return MantleParams;
}

FTransform UParkourMovementComponent::CalculateStarttingOffset(TWeakObjectPtr<UPrimitiveComponent> HitComponent, FTransform MantleLedgeWSTransform)
{
    return TransformMinusTransform(GetCharacter()->GetActorTransform(), MantleLedgeWSTransform);
}

FTransform UParkourMovementComponent::CalculateAnimatedStartOffset(FTransform MantleLedgeWSTransform)
{
    FVector PreVector = FVector(FVector(MantleLedgeWSTransform.Rotator().Vector() * MantleParams.StartingOffset.Y).X,
        FVector(MantleLedgeWSTransform.Rotator().Vector() * MantleParams.StartingOffset.Y).Y,
        MantleParams.StartingOffset.Z);
   
    return TransformMinusTransform(FTransform(MantleLedgeWSTransform.Rotator(), MantleLedgeWSTransform.GetLocation() - PreVector), MantleLedgeWSTransform);
}

FTransform UParkourMovementComponent::TransformMinusTransform(FTransform TransformA, FTransform TransformB)
{
    return FTransform(FRotator(TransformA.Rotator().Roll - TransformB.Rotator().Roll,
        TransformA.Rotator().Pitch - TransformB.Rotator().Pitch, 
        TransformA.Rotator().Yaw - TransformB.Rotator().Yaw), 
        TransformA.GetLocation() - TransformB.GetLocation(),
        TransformA.GetScale3D() - TransformB.GetScale3D());
}

FTransform UParkourMovementComponent::ComponentLocalToWorld()
{
    return MantleLedgeLS.MantleLedgeLSTransform * MantleLedgeLS.HitComponentLS->GetComponentToWorld();
}

void UParkourMovementComponent::ClearAndSetMantleState()
{
    SetMovementMode(EMovementMode::MOVE_None);
    SetMovementState(EMovementState::Action);
}

void UParkourMovementComponent::ConfiurateMantleTimeline()
{
    float MaxTime, MinTime;
    MantleParams.CorrectionCurve->GetTimeRange(MinTime, MaxTime);
    GetCharacter()->GetTimeline()->SetTimelineLength(MaxTime - MantleParams.StartingPosition);
    GetCharacter()->GetTimeline()->SetPlayRate(MantleParams.PlayRate);
    GetCharacter()->GetTimeline()->PlayFromStart();
}

void UParkourMovementComponent::PlayAnimMontageIfValid()
{
    if (!MantleParams.AnimMontage) return;
    GetCharacter()->GetMesh()->GetAnimInstance()->Montage_Play(MantleParams.AnimMontage, MantleParams.PlayRate, EMontagePlayReturnType::MontageLength, MantleParams.StartingPosition, false);
}

void UParkourMovementComponent::UpdatePositionAndCorrectionAlpha(float& PositionAlpha, float& XYCorrectionAlpha, float& ZCorrectionAlpha)
{
    PositionAlpha = (MantleParams.CorrectionCurve->GetVectorValue(GetCharacter()->GetTimeline()->GetPlaybackPosition() + MantleParams.StartingPosition)).X;
    XYCorrectionAlpha = (MantleParams.CorrectionCurve->GetVectorValue(GetCharacter()->GetTimeline()->GetPlaybackPosition() + MantleParams.StartingPosition)).Y;
    ZCorrectionAlpha = (MantleParams.CorrectionCurve->GetVectorValue(GetCharacter()->GetTimeline()->GetPlaybackPosition() + MantleParams.StartingPosition)).Z;
}

FTransform UParkourMovementComponent::CalculateLarpedTarget(FTransform MantleTargetTransform, float PositionAlpha, float XYCorrectionAlpha, float ZCorrectionAlpha, const float TimelineValue)
{
    FTransform Result;
    FTransform PreResult;
    PreResult.Blend(
        TransformPlusTransform(MantleTargetTransform, 
                                FTransform(BlendIntoTheAnimatedHorizontal(XYCorrectionAlpha).Rotator(),
                                    FVector(BlendIntoTheAnimatedHorizontal(XYCorrectionAlpha).GetLocation().X,
                                    BlendIntoTheAnimatedHorizontal(XYCorrectionAlpha).GetLocation().Y,
                                    BlendIntoTheAnimatedHorizontal(XYCorrectionAlpha).GetLocation().Z))),
        MantleTargetTransform, 
        PositionAlpha);
    Result.Blend(
        TransformPlusTransform(MantleTargetTransform, MantleActualStartOffset),
        PreResult, 
        TimelineValue);

    return Result;
}

FTransform UParkourMovementComponent::BlendIntoTheAnimatedHorizontal(float XYCorrectionAlpha)
{
    FTransform Result;
    Result.Blend(MantleActualStartOffset,
        FTransform(MantleAnimatedStartOffset.Rotator(),
        FVector(MantleAnimatedStartOffset.GetLocation().X,
            MantleAnimatedStartOffset.GetLocation().Y,
            MantleActualStartOffset.GetLocation().Z)), XYCorrectionAlpha);
    return Result;
}

FTransform UParkourMovementComponent::BlendIntoTheAnimatedVertical(float ZCorrectionAlpha)
{
    FTransform Result;
    Result.Blend(MantleActualStartOffset,
        FTransform(MantleActualStartOffset.Rotator(),
            FVector(MantleActualStartOffset.GetLocation().X,
                MantleActualStartOffset.GetLocation().Y,
                MantleAnimatedStartOffset.GetLocation().Z)), ZCorrectionAlpha);
    return Result;
}

FTransform UParkourMovementComponent::TransformPlusTransform(FTransform TransformA, FTransform TransformB)
{
    return FTransform(FRotator(TransformA.Rotator().Roll + TransformB.Rotator().Roll,
        TransformA.Rotator().Pitch + TransformB.Rotator().Pitch,
        TransformA.Rotator().Yaw + TransformB.Rotator().Yaw),
        TransformA.GetLocation() + TransformB.GetLocation(),
        TransformA.GetScale3D() + TransformB.GetScale3D());
}

void UParkourMovementComponent::SetMovementState(EMovementState NewMovementState)
{
        LastMovementState = MovementState;
        MovementState = NewMovementState;
}
    

