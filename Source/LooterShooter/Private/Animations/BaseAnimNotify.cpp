// It's my LooterShooter Game


#include "Animations/BaseAnimNotify.h"

void UBaseAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}

