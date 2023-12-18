// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/QuestBase.h"
#include "Components/QuestComponent.h"
#include "Engine/DataTable.h"

UQuestBase::UQuestBase()
{
 
}

void UQuestBase::InitializeQuest(FName ID, UDataTable* ReciveQuestData)
{
	QuestID = ID;
	QuestDataTable = ReciveQuestData;

	if (QuestDataTable)
	{
		const FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(QuestID, QuestID.ToString());
		QuestInfoData = QuestData->QuestInfo;
		CurrentStageDetails = QuestData->QuestStageData[CurrentStage];

		CurrentObjectiveProgress.Empty();

		for (int i = 0; i < CurrentStageDetails.ObjectiveDetails.Num(); i++)
		{
			CurrentObjectiveProgress.Add(CurrentStageDetails.ObjectiveDetails[i].ObjectiveId);
		}
	}
}

void UQuestBase::OnObjectiveIdHeard(FName ID)
{
	if (int32* i = CurrentObjectiveProgress.Find(ID))
	{
		int32 k = *i + 1;
		CurrentObjectiveProgress.Add(ID, k);
	}
}

