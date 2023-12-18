// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/Quest/QuestEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Data/QuestDataStructs.h"
#include "UserInterfaces/Quest/QuestLogWidget.h"
#include "Quests/QuestBase.h"

void UQuestEntryWidget::InitializeWidget(UQuestLogWidget* ParentWidget)
{
	Parent = ParentWidget;

}

void UQuestEntryWidget::UpdateWidget(FName QuestID, UQuestBase* ReciveQuestReference)
{
	ID = QuestID;
	const FQuestData* Data = QuestData->FindRow<FQuestData>(QuestID, QuestID.ToString());
	QuestName->SetText(Data->QuestInfo.QuestName);

	QuestReference = ReciveQuestReference;
}

void UQuestEntryWidget::ButtonClick()
{
	Parent->QuestSelected(ID, QuestReference);
}
