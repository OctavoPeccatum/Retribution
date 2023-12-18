// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

class ALooterShooterCharacter;
 
UENUM()
enum class EInteractableType : uint8
{
	Pickup				UMETA(DisplayName = "Pickup"),
	NonPlayerCharacter	UMETA(DisplayName = "NonPlayerCharacter"),
	Device				UMETA(DisplayName = "Device"),
	Toggle				UMETA(DisplayName = "Toggle"),
	Container			UMETA(DisplayName = "Container"),
	QuestGiver			UMETA(DisplayName = "QuestGiver")
};

USTRUCT()
struct FInteractableData
{
	GENERATED_USTRUCT_BODY()

	FInteractableData() : InteractableType(EInteractableType::Pickup), 
		Name(FText::GetEmpty()), 
		Action(FText::GetEmpty()), 
		Quantity(0), 
		InteractionDuration(0.0f)
	{

	};

	UPROPERTY(EditInstanceOnly)
	EInteractableType InteractableType;
	
	UPROPERTY(EditInstanceOnly)
	FText Name;

	UPROPERTY(EditInstanceOnly)
	FText Action;

	// Use only for PickUps
	UPROPERTY(EditInstanceOnly)
	int8 Quantity;

	// Use for Interaction Timer
	UPROPERTY(EditInstanceOnly)
	float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOOTERSHOOTER_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void BeginFocus();
	virtual void EndFocus();
	virtual void BeginInteract();
	virtual void EndInteract();
	virtual FName Interact(ALooterShooterCharacter* PLayerCharacter);

	FInteractableData InteractableData;
};
