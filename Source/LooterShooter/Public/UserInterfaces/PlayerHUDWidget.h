// It's my LooterShooter Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LooterShooterCoreTypes.h"
#include "PlayerHUDWidget.generated.h"

class UWeaponComponent;
/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetWeaponUIData(FWeaponUIData& UIData) const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

private:
	UWeaponComponent* GetWeaponComponent() const;

};
