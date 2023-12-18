// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/QuestComponent.h"
#include "Quests/QuestBase.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UQuestComponent::AddNewQuest(FName QuestID)
{
	CurrentActiveQuests.AddUnique(QuestID);
	UQuestBase* NewQuest = NewObject<UQuestBase>(this, BaseClass);
	NewQuest->InitializeQuest(QuestID, QuestDataTable);
	CurrentQuest.Add(NewQuest);
}

void UQuestComponent::CompleteQuest()
{
}

bool UQuestComponent::QueryActiveQuest(FName QuestID)
{
	if (CurrentActiveQuests.Contains(QuestID))
	{
		return true;
	}

	return false;
}

void UQuestComponent::TrackQuest()
{
}


// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

