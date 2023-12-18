// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class ALooterShooterCharacter;
class UInventoryPanel;
class UQuestLogWidget;

UCLASS()
class LOOTERSHOOTER_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	ALooterShooterCharacter* PlayerCharacter;

	void ToggleInventory();
	void ToggleQuestLog();
	void HideAll();

protected:

	bool bIsInventoryVisible;
	bool bIsQuestLogVisible;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UInventoryPanel* InventoryPanel;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UQuestLogWidget* QuestLog;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
