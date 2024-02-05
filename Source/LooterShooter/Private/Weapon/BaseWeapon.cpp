// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BaseWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultsAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
	checkf(DefaultsAmmo.ReservBullets > 0, TEXT("Clips count couldn't be less or equal zero"));
	CurrentAmmo = DefaultsAmmo;
}

void ABaseWeapon::MakeShot()
{

}

void ABaseWeapon::StartFire()
{

}

void ABaseWeapon::StopFire()
{

}

APlayerController* ABaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>();
}

bool ABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetPlayerController();
	if (!Controller) return false;

	Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

FVector ABaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

bool ABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}

void ABaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
	if (!GetWorld()) return;
	FCollisionQueryParams CollisionParam;
	CollisionParam.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParam);
}

void ABaseWeapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}

void ABaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets == 0)
	{
		UE_LOG(BaseWeaponLog, Warning, TEXT("Clip is empty!"));
		return;
	}

	CurrentAmmo.Bullets--;
	LogAmmo();

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast();
	}
}

bool ABaseWeapon::IsAmmoEmpty()
{
	return CurrentAmmo.ReservBullets == 0 && IsClipEmpty();
}

bool ABaseWeapon::IsClipEmpty()
{
	return CurrentAmmo.Bullets == 0;
}

void ABaseWeapon::ChangeClip()
{
	if (CurrentAmmo.ReservBullets == 0)
	{
		UE_LOG(BaseWeaponLog, Warning, TEXT("No more clips!"));
		return;
	}

	CalculateAmmo();
	UE_LOG(BaseWeaponLog, Display, TEXT("---------Change Clip!-----------"));
}

bool ABaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultsAmmo.Bullets && CurrentAmmo.ReservBullets > 0;
}

void ABaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += FString::FromInt(CurrentAmmo.ReservBullets);
	UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo);
}

void ABaseWeapon::CalculateAmmo()
{
	CurrentAmmo.ReservBullets += CurrentAmmo.Bullets;
	CurrentAmmo.Bullets = FMath::Min(CurrentAmmo.ReservBullets, DefaultsAmmo.Bullets);
	CurrentAmmo.ReservBullets = FMath::Max(CurrentAmmo.ReservBullets - CurrentAmmo.Bullets, 0);
}

