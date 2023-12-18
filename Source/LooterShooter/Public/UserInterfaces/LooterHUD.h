// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LooterHUD.generated.h"

struct FInteractableData;
struct FQuestData;
class UInteractionWidget;
class UMainMenu;
class UQuestGiveWidget;
class UQuestLogWidget;

UCLASS()
class LOOTERSHOOTER_API ALooterHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UQuestGiveWidget> QuestGiveWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UQuestLogWidget> QuestLogWidgetClass;

	bool bIsMenuVisible;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	ALooterHUD();

	void DisplayInventory();
	void HideInventory();
	void ToggleInventory();

	void DisplayQuestLog();
	void HideQuestLog();
	void ToggleQuestLog();

	void HideInvAndQuest();

	void ShowInteractionWidget() const;
	void HideInteractionWidget() const;
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

	void ShowQuestGiveWidget();
	void HideQuestGiveWidget() const;
	void UpdateQuestGiveWidget(const FQuestData* QuestData) const;

protected:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY()
	UMainMenu* MainMenuWidget;


	UPROPERTY()
	UInteractionWidget* InteractionWidget;

	UPROPERTY()
	UQuestGiveWidget* QuestGiveWidget;

	UPROPERTY()
	UQuestLogWidget* QuestLogWidget;


	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	virtual void BeginPlay() override;
};
