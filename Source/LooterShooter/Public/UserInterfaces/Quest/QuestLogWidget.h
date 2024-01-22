// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestLogWidget.generated.h"

class UTextBlock;
class ALooterShooterCharacter;
class UQuestComponent;
class UQuestEntryWidget;
class UQuestObjective;
class UVerticalBox;
class UScrollBox;
class UWidgetSwitcher;
class UDataTable;
class UQuestBase;

/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UQuestLogWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere)
	ALooterShooterCharacter* PlayerReference;

	UPROPERTY(VisibleAnywhere)
	UQuestComponent* PlayerQuestComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UQuestEntryWidget> EntryWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UQuestObjective> QuestObjectiveClass;

	void UpdateWidget();
	void QuestSelected(FName QuestID, UQuestBase* QuestReference);

protected:

	UPROPERTY(EditAnywhere)
	UDataTable* QuestData;

	UQuestBase* CurrentQuestReference;

	UPROPERTY()
	UQuestEntryWidget* EntryWidget;

	UPROPERTY()
	UQuestObjective* QuestObjectiveWidget;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* QuestDescription;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* StageDescription;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UScrollBox* ScrollMain;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UScrollBox* ScrollSide;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UVerticalBox* ObjectivesBox;

	void InitializeWidget();
	//virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//virtual void NativePreConstruct() override;
	void DisplayQuest(FName QuestID, UQuestBase* QuestReference);

};
