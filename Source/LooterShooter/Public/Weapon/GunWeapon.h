// It's my LooterShooter Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "GunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API AGunWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BulletSpread = 1.5f;

	virtual void MakeShot() override;

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:

	FTimerHandle ShotTimerHandle;
};
