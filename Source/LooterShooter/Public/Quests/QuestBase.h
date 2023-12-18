// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/QuestDataStructs.h"
#include "/UEProject/LooterShooter/Source/LooterShooter/LooterShooterCharacter.h"
#include "QuestBase.generated.h"

class UQuestComponent;
class UDataTable;
/**
 * 
 */
UCLASS()
class LOOTERSHOOTER_API UQuestBase : public UObject
{
	GENERATED_BODY()
	
public:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY(VisibleAnywhere, Category = "Quest")
	FName QuestID;

	UPROPERTY(VisibleAnywhere, Category = "Quest Data")
	FQuestDetails QuestInfoData;

	UPROPERTY(VisibleAnywhere, Category = "Quest Data")
	FQuestReward QuestRewardData;

	UPROPERTY(VisibleAnywhere, Category = "Quest Data")
	TArray<FQuestStageDetails> QuestStageData;

	UPROPERTY(VisibleAnywhere)
	TMap<FName, int32> CurrentObjectiveProgress;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
	UQuestBase();
	void InitializeQuest(FName ID, UDataTable* ReciveQuestData);
	void OnObjectiveIdHeard(FName ID);

protected:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY()
	UQuestComponent* OwningQuestComponent;

	UPROPERTY(VisibleAnywhere, Category = "Quest | Quest Data")
	UDataTable* QuestDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Quest | Quest Data")
	FQuestStageDetails CurrentStageDetails;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentStage;

	bool bIsCompleted;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

};
