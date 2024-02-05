// It's my LooterShooter Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LooterShooterCoreTypes.h"
#include "WeaponComponent.generated.h"

class ABaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTERSHOOTER_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();
	void Reload();

	bool GetWeaponUIData(FWeaponUIData& UIData) const;
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;
	bool GetWeaponIsAnimatingNow() const { return bWeaponActionInProgress; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	bool bWeaponActionInProgress = false;

	UPROPERTY()
	ABaseWeapon* CurrentWeapon = nullptr;

	UPROPERTY()
	ABaseWeapon* LastWeapon = nullptr;

	UPROPERTY()
	TArray<ABaseWeapon*> Weapons;

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimMontage = nullptr;

	int32 CurrentWeaponIndex = 0;
	bool EquipAnimInProgress = false;
	bool ReloadAnimInProgress = false;

	void InitAnimations();
	void SpawnWeapons();
	void AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation);

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);
	void OnChangeWeapon(USkeletalMeshComponent* MeshComp);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReload() const;

	void OnEmptyClip();
	void ChangeClip();

};
