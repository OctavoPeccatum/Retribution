// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestObjective.generated.h"

class UTextBlock;
class UCheckBox;
struct FObjectiveDetails;
class UQuestBase;
/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UQuestObjective : public UUserWidget
{
	GENERATED_BODY()
	
public:

	const FObjectiveDetails* ObjectiveData;

	void InitializeWidget(UQuestBase* CurrentQuestReference);

protected:

	UQuestBase* QuestReference;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UTextBlock* ObjDescription;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UCheckBox* CheckIsCompleted;
};
