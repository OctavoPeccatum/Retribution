// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/Quest/QuestLogWidget.h"
#include "UserInterfaces/Quest/QuestEntryWidget.h"
#include "UserInterfaces/Quest/QuestObjective.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "LooterShooter/LooterShooterCharacter.h"
#include "Components/QuestComponent.h"
#include "Data/QuestDataStructs.h"
#include "Quests/QuestBase.h"

void UQuestLogWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitializeWidget();
}

void UQuestLogWidget::InitializeWidget()
{
	PlayerReference = Cast<ALooterShooterCharacter>(GetOwningPlayerPawn());
	if (PlayerReference)
	{
		PlayerQuestComponent = PlayerReference->GetQuestComponent();
	}
}

void UQuestLogWidget::UpdateWidget()
{
	ScrollMain->ClearChildren();

	if (PlayerQuestComponent)
	{
		if (PlayerQuestComponent->CurrentQuest.Num() >= 1)
		{
			for (int32 i = 0; i < PlayerQuestComponent->CurrentQuest.Num(); i++)
			{
				if (EntryWidgetClass)
				{
					EntryWidget = CreateWidget< UQuestEntryWidget>(GetWorld(), EntryWidgetClass);
					FName WidgetQuestID = PlayerQuestComponent->CurrentQuest[i]->QuestID;
					const FQuestData* Data = QuestData->FindRow<FQuestData>(WidgetQuestID, WidgetQuestID.ToString());

					if (Data->QuestInfo.bIsMainQuest)
					{
						ScrollMain->AddChild(EntryWidget);
					}
					else
					{
						ScrollSide->AddChild(EntryWidget);
					}

					EntryWidget->InitializeWidget(this);
					EntryWidget->UpdateWidget(WidgetQuestID, PlayerQuestComponent->CurrentQuest[i]);
				}
			}

			//DisplayQuest(PlayerQuestComponent->CurrentActiveQuests[0]);
		}
	}
	
}

void UQuestLogWidget::DisplayQuest(FName QuestID, UQuestBase* QuestReference)
{
	CurrentQuestReference = QuestReference;

	WidgetSwitcher->SetActiveWidgetIndex(1);
	ObjectivesBox->ClearChildren();

	const FQuestData* Data = QuestData->FindRow<FQuestData>(QuestID, QuestID.ToString());
	QuestName->SetText(Data->QuestInfo.QuestName);
	QuestDescription->SetText(Data->QuestInfo.QuestDescription);
	StageDescription->SetText(Data->QuestStageData[0].StageDescription);

	if (QuestObjectiveClass)
	{
		for (int i = 0; i < Data->QuestStageData.Num(); i++)
		{
			for (int k = 0; k < Data->QuestStageData[i].ObjectiveDetails.Num(); k++)
			{
				QuestObjectiveWidget = CreateWidget<UQuestObjective>(GetWorld(), QuestObjectiveClass);
				QuestObjectiveWidget->ObjectiveData = &Data->QuestStageData[i].ObjectiveDetails[k];
				QuestObjectiveWidget->InitializeWidget(CurrentQuestReference); 
				ObjectivesBox->AddChildToVerticalBox(QuestObjectiveWidget);
			}
		}
	}
}


void UQuestLogWidget::QuestSelected(FName QuestID, UQuestBase* QuestReference)
{
	DisplayQuest(QuestID, QuestReference);
}
