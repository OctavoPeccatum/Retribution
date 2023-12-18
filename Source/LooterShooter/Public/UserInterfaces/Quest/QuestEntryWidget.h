// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestEntryWidget.generated.h"

class UButton;
class UTextBlock;
class UCheckBox;
class UQuestLogWidget;
class UDataTable;
/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UQuestEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void UpdateWidget(FName QuestID, UQuestBase* ReciveQuestReference);
	void InitializeWidget(UQuestLogWidget* ParentWidget);

protected:

	UPROPERTY(EditAnywhere)
	UDataTable* QuestData;

	UQuestBase* QuestReference;

	FName ID;

	UQuestLogWidget* Parent;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UButton* Button;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* QuestName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCheckBox* CheckIsCompleted;

	UFUNCTION(BlueprintCallable)
	void ButtonClick();

	//virtual void NativeOnInitialized() override;
	//virtual void NativeConstruct() override;

};
