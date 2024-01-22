// It's my LooterShooter Game


#include "UserInterfaces/PlayerHUDWidget.h"
#include "Components/HealthComponent.h"

float UPlayerHUDWidget::GetHealthPercent() const
{
	const auto Player = GetOwningPlayerPawn();
	if (!Player) return 0.0f;

	const auto Component = Player->GetComponentByClass(UHealthComponent::StaticClass());
	const auto HealthComponent = Cast<UHealthComponent>(Component);
	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
}
