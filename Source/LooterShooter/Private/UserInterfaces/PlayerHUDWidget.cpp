// It's my LooterShooter Game


#include "UserInterfaces/PlayerHUDWidget.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "Weapon/BaseWeapon.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return 0.0f;

	const auto Component = Player->GetComponentByClass(UHealthComponent::StaticClass());
	const auto HealthComponent = Cast<UHealthComponent>(Component);
	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}

bool UPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent) return false;

	return WeaponComponent->GetWeaponUIData(UIData);
}

bool UPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = GetWeaponComponent();
	if (!WeaponComponent) return false;
	
	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

UWeaponComponent* UPlayerHUDWidget::GetWeaponComponent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return nullptr;

	const auto Component = Player->GetComponentByClass(UWeaponComponent::StaticClass());
	const auto WeaponComponent = Cast<UWeaponComponent>(Component);
	return WeaponComponent;
}
