// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestGiverComponent.generated.h"

class UQuestBase;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTERSHOOTER_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	// Sets default values for this component's properties
	UQuestGiverComponent();

	void InitializedQuest(const TSubclassOf<UQuestBase> BaseClass);
	void PerformQuest(ALooterShooterCharacter* PlayerCharacter);
	void DisplayQuest(ALooterHUD* HUD);

protected:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY(EditInstanceOnly, Category = "Quest | Quest Initialization")
	UDataTable* QuestDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Quest | Quest Initialization")
	FName DesiredQuestID;

	UPROPERTY(VisibleAnywhere, Category = "Quest | Quest Reference")
	UQuestBase* QuestReference;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	// Called when the game starts
	virtual void BeginPlay() override;		
};
