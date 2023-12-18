// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/Quest/QuestObjective.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "Data/QuestDataStructs.h"
#include <Quests/QuestBase.h>

void UQuestObjective::InitializeWidget(UQuestBase* CurrentQuestReference)
{
	QuestReference = CurrentQuestReference;

	ObjDescription->SetText(FText::Format(FText::FromString("{0} {1}/{2}"),
		ObjectiveData->ObjectiveDescription,
		0,
		ObjectiveData->Quantity));

	CheckIsCompleted->SetCheckedState(ECheckBoxState::Unchecked);
}
