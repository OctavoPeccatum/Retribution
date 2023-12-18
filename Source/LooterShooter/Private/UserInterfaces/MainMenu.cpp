// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/MainMenu.h"
#include "Items/ItemBase.h"
#include "UserInterfaces/Inventory/ItemDragDropOperation.h"
#include "LooterShooter/LooterShooterCharacter.h"
#include "UserInterfaces/Inventory/InventoryPanel.h"
#include "UserInterfaces/Quest/QuestLogWidget.h"

void UMainMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ALooterShooterCharacter>(GetOwningPlayerPawn());
	bIsInventoryVisible = false;
	bIsQuestLogVisible = false;
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

	if (PlayerCharacter && ItemDragDrop->SourceItem )
	{
		PlayerCharacter->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
		return true;
	}
	return false;
}

void UMainMenu::ToggleInventory()
{
	if (bIsInventoryVisible)
	{
		HideAll();
	}
	else
	{
		bIsInventoryVisible = true;
		InventoryPanel->SetVisibility(ESlateVisibility::Visible);
	}

}

void UMainMenu::ToggleQuestLog()
{
	if (bIsQuestLogVisible)
	{
		HideAll();
	}
	else
	{
		bIsQuestLogVisible = true;
		QuestLog->SetVisibility(ESlateVisibility::Visible);
		QuestLog->UpdateWidget();
	}
}

void UMainMenu::HideAll()
{
	bIsQuestLogVisible = false;
	QuestLog->SetVisibility(ESlateVisibility::Collapsed);

	bIsInventoryVisible = false;
	InventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
}
