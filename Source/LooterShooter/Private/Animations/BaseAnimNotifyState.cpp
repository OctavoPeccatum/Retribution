// It's my LooterShooter Game


#include "Animations/BaseAnimNotifyState.h"

void UBaseAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	OnNotifiedStart.Broadcast(MeshComp, bNeedCollisionDisable);
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
}

void UBaseAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotifiedEnd.Broadcast(MeshComp, bNeedCollisionDisable);
	Super::NotifyEnd(MeshComp, Animation);
}
