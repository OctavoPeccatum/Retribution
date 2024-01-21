// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestComponent.generated.h"

class UQuestBase;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTERSHOOTER_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY(VisibleAnywhere)
	TArray<FName> CurrentActiveQuests;

	UPROPERTY(VisibleAnywhere)
	TArray<FName> CompletedQuests;

	UPROPERTY(VisibleAnywhere)
	FText CurrentTrackedQuest;

	UPROPERTY(VisibleAnywhere)
	TArray<UQuestBase*> CurrentQuest;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UQuestBase> BaseClass;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	// Sets default values for this component's properties
	UQuestComponent();

	bool QueryActiveQuest(FName QuestID);

	void AddNewQuest(FName QuestID);

protected:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY(EditDefaultsOnly, Category = "Quest | Quest Data")
	UDataTable* QuestDataTable;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	void CompleteQuest();
	void TrackQuest();

	// Called when the game starts
	virtual void BeginPlay() override;


		
};
