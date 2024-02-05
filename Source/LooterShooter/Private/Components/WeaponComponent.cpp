// It's my LooterShooter Game


#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animations/EquipFinishAnimNotify.h"
#include "Animations/ReloadFinishedAnimNotify.h"
#include "Animations/ChangeWeaponAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

constexpr static int32 WeaponNum = 2;

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items!"), WeaponNum);

	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon : Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	for (auto OneWeaponData : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->OnClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UWeaponComponent::AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* SceneComponent, FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void UWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(WeaponComponentLog, Warning, TEXT("Invalid weapon index!"));
		return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		LastWeapon = CurrentWeapon;
	}

	CurrentWeapon = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {//
		return Data.WeaponClass == CurrentWeapon->GetClass(); //
		});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	EquipAnimInProgress = true;
	PlayAnimMontage(EquipAnimMontage);
}	

void UWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	bWeaponActionInProgress = true;
	CurrentWeapon->StartFire();
}

void UWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	bWeaponActionInProgress = false;
	CurrentWeapon->StopFire();
}

void UWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;

	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::Reload()
{
	ChangeClip();
}

void UWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}

void UWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
	}
	else
	{
		UE_LOG(WeaponComponentLog, Error, TEXT("Equip anim notify is forgotten to set!"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(WeaponComponentLog, Error, TEXT("Reload anim notify is forgotten to set!"));
			checkNoEntry();
		}

		ReloadFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnReloadFinished);
		
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ChangeWeaponNotify = AnimUtils::FindNotifyByClass<UChangeWeaponAnimNotify>(EquipAnimMontage);
		if (!ChangeWeaponNotify)
		{
			UE_LOG(WeaponComponentLog, Error, TEXT("Change anim notify is forgotten to set!"));
			checkNoEntry();
		}

		ChangeWeaponNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnChangeWeapon);

	}
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	EquipAnimInProgress = false;
}

void UWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	ReloadAnimInProgress = false;
	bWeaponActionInProgress = false;
}

void UWeaponComponent::OnChangeWeapon(USkeletalMeshComponent* MeshComp)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh() != MeshComp) return;

	AttachWeaponToSocket(LastWeapon, Character->GetMesh(), WeaponArmorySocketName);
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
}

bool UWeaponComponent::CanFire() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UWeaponComponent::CanReload() const
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}


bool UWeaponComponent::CanEquip() const
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

void UWeaponComponent::OnEmptyClip()
{
	ChangeClip();
}

void UWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	bWeaponActionInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}

bool UWeaponComponent::GetWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

