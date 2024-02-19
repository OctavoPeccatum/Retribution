// It's my LooterShooter Game

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "BaseAnimNotifyState.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNotifiedStartSignature, USkeletalMeshComponent*, bool);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnNotifiedEndSignature, USkeletalMeshComponent*, bool);

UCLASS()
class LOOTERSHOOTER_API UBaseAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
public: 
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	FOnNotifiedStartSignature OnNotifiedStart;
	FOnNotifiedEndSignature OnNotifiedEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimNotify)
	bool bNeedCollisionDisable = false;
};
