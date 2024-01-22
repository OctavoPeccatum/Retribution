// It's my LooterShooter Game

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API ARifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public: 
	virtual void StartFire() override;

protected:
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
};
