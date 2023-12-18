// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractionInterface.h"
#include "QuestGiver.generated.h"

class UQuestGiverComponent;

UCLASS()
class LOOTERSHOOTER_API AQuestGiver : public AActor, public IInteractionInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AQuestGiver();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

protected:

	UPROPERTY(EditInstanceOnly, Category = "Interact | Item Initialization")
	FName ID;

	UPROPERTY(VisibleAnywhere, Category = "Quest Giver | Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Quest Giver | Components")
	UQuestGiverComponent* QuestGiverComponent;

	UPROPERTY(EditAnywhere, Category = "Quest Giver | Interaction")
	FInteractableData InstanceInteractableData;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual FName Interact(ALooterShooterCharacter* PlayerCharacter) override;
};