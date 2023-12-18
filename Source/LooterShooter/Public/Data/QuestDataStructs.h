// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestDataStructs.generated.h"

UENUM()
enum class EObjectiveType : uint8
{
	Location	UMETA(DisplayName = "Location"),
	Destroy		UMETA(DisplayName = "Destroy"),
	Kill		UMETA(DisplayName = "Kill"),
	Interact	UMETA(DisplayName = "Interact"),
	Collect		UMETA(DisplayName = "Collect"),
	Escort		UMETA(DisplayName = "Escort")
};

USTRUCT()
struct FQuestDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText QuestName;

	UPROPERTY(EditAnywhere)
	FText QuestDescription;

	UPROPERTY(EditAnywhere)
	FText TrackDescription;

	UPROPERTY(EditAnywhere)
	bool bIsMainQuest;
};

USTRUCT()
struct FQuestReward
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32 ExpReward;

	UPROPERTY(EditAnywhere)
	FText ItemReward;

	UPROPERTY(EditAnywhere)
	int32 GoldRevard;
};

USTRUCT()
struct FObjectiveDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText ObjectiveName;

	UPROPERTY(EditAnywhere)
	FText ObjectiveDescription;

	UPROPERTY(EditAnywhere)
	EObjectiveType ObjectiveType;

	UPROPERTY(EditAnywhere)
	FName ObjectiveId;

	UPROPERTY(EditAnywhere)
	int32 Quantity;

	UPROPERTY(EditAnywhere)
	bool bIsOptional;

	UPROPERTY(EditAnywhere)
	int32 BonusXP;
};

USTRUCT()
struct FQuestStageDetails
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	int32 StageNum;

	UPROPERTY(EditAnywhere)
	FText StageName;

	UPROPERTY(EditAnywhere)
	FText StageDescription;

	UPROPERTY(EditAnywhere)
	TArray<FObjectiveDetails> ObjectiveDetails;
};


USTRUCT()
struct FQuestData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Quest Data")
	FName QuestID;

	UPROPERTY(EditAnywhere, Category = "Quest Data")
	FQuestDetails QuestInfo;

	UPROPERTY(EditAnywhere, Category = "Quest Data")
	FQuestReward QuestRewardData;

	UPROPERTY(EditAnywhere, Category = "Quest Data")
	TArray<FQuestStageDetails> QuestStageData;
};

UCLASS()
class LOOTERSHOOTER_API UQuestDataStructs : public UDataTable
{
	GENERATED_BODY()
	
};
