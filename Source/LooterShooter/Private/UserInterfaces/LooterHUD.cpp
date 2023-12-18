// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/LooterHUD.h"
#include "UserInterfaces/MainMenu.h"
#include "UserInterfaces/Quest/QuestGiveWidget.h"
#include "LooterShooter/Public/InteractionWidget.h"
#include "UserInterfaces/Quest/QuestLogWidget.h"

ALooterHUD::ALooterHUD()
{
}

void ALooterHUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainMenuClass)
	{
		MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (QuestGiveWidgetClass)
	{
		QuestGiveWidget = CreateWidget<UQuestGiveWidget>(GetWorld(), QuestGiveWidgetClass);
		QuestGiveWidget->AddToViewport(-1);
		QuestGiveWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (QuestLogWidgetClass)
	{
		QuestLogWidget = CreateWidget<UQuestLogWidget>(GetWorld(), QuestLogWidgetClass);
		QuestLogWidget->AddToViewport(-1);
		QuestLogWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALooterHUD::DisplayInventory()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
		MainMenuWidget->ToggleInventory();
	}
}

void ALooterHUD::HideInventory()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		MainMenuWidget->ToggleInventory();
	}
}

void ALooterHUD::ToggleInventory()
{
	if (bIsMenuVisible)
	{
		HideInvAndQuest();
	}
	else
	{
		DisplayInventory();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ALooterHUD::DisplayQuestLog()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
		MainMenuWidget->ToggleQuestLog();
	}
}

void ALooterHUD::HideQuestLog()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		MainMenuWidget->ToggleQuestLog();
	}
}

void ALooterHUD::ToggleQuestLog()
{
	if (bIsMenuVisible)
	{
		HideInvAndQuest();
	}
	else
	{
		DisplayQuestLog();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void ALooterHUD::HideInvAndQuest()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
		MainMenuWidget->HideAll();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
}

void ALooterHUD::ShowInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ALooterHUD::HideInteractionWidget() const
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALooterHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}

void ALooterHUD::ShowQuestGiveWidget()
{
	if (QuestGiveWidget)
	{
		if (bIsMenuVisible)
		{
			HideInvAndQuest();
		}
			const FInputModeUIOnly InputMode;
			GetOwningPlayerController()->SetInputMode(InputMode);
			GetOwningPlayerController()->SetShowMouseCursor(true);

			QuestGiveWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ALooterHUD::HideQuestGiveWidget() const
{
	if (QuestGiveWidget)
	{
			const FInputModeGameOnly InputMode;
			GetOwningPlayerController()->SetInputMode(InputMode);
			GetOwningPlayerController()->SetShowMouseCursor(false);

			QuestGiveWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ALooterHUD::UpdateQuestGiveWidget(const FQuestData* QuestData) const
{
	if (QuestGiveWidget)
	{
		if (QuestGiveWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			QuestGiveWidget->SetVisibility(ESlateVisibility::Visible);
		}

		QuestGiveWidget->UpdateWidget(QuestData);
	}
}


