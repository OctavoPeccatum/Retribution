// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestGiverComponent.h"
#include "Components/QuestComponent.h"
#include "UserInterfaces/LooterHUD.h"
#include "Quests/QuestBase.h"

// Sets default values for this component's properties
UQuestGiverComponent::UQuestGiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UQuestGiverComponent::BeginPlay()
{
	Super::BeginPlay();

	InitializedQuest(UQuestBase::StaticClass());
	
}

void UQuestGiverComponent::InitializedQuest(const TSubclassOf<UQuestBase> BaseClass)
{
	if (QuestDataTable && !DesiredQuestID.IsNone())
	{
		const FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(DesiredQuestID, DesiredQuestID.ToString());

		QuestReference = NewObject<UQuestBase>(this, BaseClass);

		QuestReference->QuestID = QuestData->QuestID;
		QuestReference->QuestInfoData = QuestData->QuestInfo;
		QuestReference->QuestRewardData = QuestData->QuestRewardData;
		QuestReference->QuestStageData = QuestData->QuestStageData;
	}
}

void UQuestGiverComponent::PerformQuest(ALooterShooterCharacter* PlayerCharacter)
{
	if (!PlayerCharacter->GetQuestComponent()->QueryActiveQuest(DesiredQuestID))
	{
		DisplayQuest(PlayerCharacter->GetHUD());
	}
}

void UQuestGiverComponent::DisplayQuest(ALooterHUD* HUD) 
{
	const FQuestData* QuestData = QuestDataTable->FindRow<FQuestData>(DesiredQuestID, DesiredQuestID.ToString());

	
	HUD->UpdateQuestGiveWidget(QuestData);
	HUD->ShowQuestGiveWidget();
}




