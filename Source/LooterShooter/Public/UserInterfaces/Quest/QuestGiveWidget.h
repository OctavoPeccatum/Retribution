// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestGiveWidget.generated.h"

struct FQuestData;
class UTextBlock;
class UButton;
class UQuestBase;
class UQuestObjective;
class UVerticalBox;

UCLASS()
class LOOTERSHOOTER_API UQuestGiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere, Category = "Quest Widget | Player Reference")
	ALooterShooterCharacter* PlayerReference;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UQuestObjective> QuestObjectiveClass;

	void UpdateWidget(const FQuestData* QuestData);

protected:

	FName QuestID;

	ALooterHUD* HUD;

	UPROPERTY()
	UQuestObjective* QuestObjectiveWidget;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Quest Widget | Quest Data")
	UTextBlock* QuestName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Quest Widget | Quest Data")
	UTextBlock* QuestDescription;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Quest Widget | Quest Data")
	UTextBlock* StageDescription;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Quest Widget | Button")
	UButton* ButtonAccept;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Quest Widget | Button")
	UButton* ButtonDecline;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UVerticalBox* ObjectivesBox;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable)
	void AcceptQuest();

	UFUNCTION(BlueprintCallable)
	void DeclineQuest();
};
