// Fill out your copyright notice in the Description page of Project Settings.


#include "UserInterfaces/Quest/QuestGiveWidget.h"
#include "UserInterfaces/LooterHUD.h"
#include "UserInterfaces/Quest/QuestObjective.h"
#include "LooterShooter/LooterShooterCharacter.h"
#include "Components/QuestComponent.h"
#include "UObject/NameTypes.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Quests/QuestBase.h"

void UQuestGiveWidget::UpdateWidget(const FQuestData* QuestData)
{
	ObjectivesBox->ClearChildren();

	QuestID = QuestData->QuestID;
	QuestName->SetText(QuestData->QuestInfo.QuestName);
	QuestDescription->SetText(QuestData->QuestInfo.QuestDescription);
	StageDescription->SetText(QuestData->QuestStageData[0].StageDescription);

	if (QuestObjectiveClass)
	{
		for (int i = 0; i < QuestData->QuestStageData.Num(); i++)
		{
			for (int k = 0; k < QuestData->QuestStageData[i].ObjectiveDetails.Num(); k++)
			{
				QuestObjectiveWidget = CreateWidget<UQuestObjective>(GetWorld(), QuestObjectiveClass);
				QuestObjectiveWidget->ObjectiveData = &QuestData->QuestStageData[i].ObjectiveDetails[k];
				QuestObjectiveWidget->InitializeWidget(nullptr);
				ObjectivesBox->AddChildToVerticalBox(QuestObjectiveWidget);
			}
		}
	}
}

void UQuestGiveWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UQuestGiveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HUD = Cast<ALooterHUD>(GetOwningPlayer()->GetHUD());
}

void UQuestGiveWidget::AcceptQuest()
{
	ALooterShooterCharacter* PlayerCharacter = Cast<ALooterShooterCharacter>(GetOwningPlayer()->GetCharacter());
	if (PlayerCharacter->GetQuestComponent())
	{
		PlayerCharacter->GetQuestComponent()->AddNewQuest(QuestID);

		if (HUD)
		{
			HUD->HideQuestGiveWidget();
		}
	}
}

void UQuestGiveWidget::DeclineQuest()
{
	if (HUD)
	{
		HUD->HideQuestGiveWidget();
	}
}
