// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/Inventory/InventoryTooltip.h"
#include "Components/TextBlock.h"
#include "UserInterfaces/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"


void UInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

	switch (ItemBeingHovered->ItemType)
	{
	case EItemType::Armor:
		ItemType->SetText(FText::FromString("Armor"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Ammo:
		ItemType->SetText(FText::FromString("Ammo"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Weapon:
		ItemType->SetText(FText::FromString("Weapon"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Consumable:
		ItemType->SetText(FText::FromString("Consumable"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EItemType::Quest:
		ItemType->SetText(FText::FromString("Quest Item"));
		DamageValue->SetVisibility(ESlateVisibility::Collapsed);
		ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		break;
	}
	
	ItemName->SetText(ItemBeingHovered->TextData.Name);
	DamageValue->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.DamageValue));
	ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->ItemStatistics.ArmorRating));
	UsageText->SetText(ItemBeingHovered->TextData.UsageText);
	ItemDescription->SetText(ItemBeingHovered->TextData.Description);

	const FString WeightInfo =
	{ "Weight: " + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight()) };

	StackWeight->SetText(FText::FromString(WeightInfo));

	if (ItemBeingHovered->NumericData.bIsStackable)
	{
		const FString StackInfo =
		{ "Max stack size: " + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize) };

		MaxStackSize->SetText(FText::FromString(StackInfo));
	}
	else
	{
		MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
	}
}
