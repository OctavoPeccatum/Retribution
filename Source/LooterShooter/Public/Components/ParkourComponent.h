// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ParkourComponent.generated.h"

class ALooterShooterCharacter;

UENUM(BlueprintType)
enum class EParkourState : uint8
{
	Running			UMETA(DisplayName = "Character is Runing"),
	Sprinting		UMETA(DisplayName = "Character is Sprinting"),
	Sliding			UMETA(DisplayName = "Character is Sliding")
};

UENUM(BlueprintType)
enum class ECharcterState : uint8
{
	Standing		UMETA(DisplayName = "Character is Standing"),
	Siting			UMETA(DisplayName = "Character is Siting"),
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTERSHOOTER_API UParkourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES


	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS
	// 
	// Sets default values for this component's properties
	UParkourComponent();

	void InitializeComponent(ALooterShooterCharacter* OwnerComponent);
	void OnSprintStart();
	void OnSprintEnd();
	void OnSitStart();
	void OnSitEnd();
	void OnOvercomming();


protected:

	/////////////////////////////////////////////////////////////////////////////////////////////
	// PROPERTIES & VARIABLES

	UPROPERTY(VisibleAnywhere, Category = "Parkour")
	EParkourState ParkourState;

	UPROPERTY(VisibleAnywhere, Category = "Parkour")
	ECharcterState CharacterState;

	/////////////////////////////////////////////////////////////////////////////////////////////
	// FUNCTIONS

	void SprintStart();
	void SprintEnd();
	void SitStart();
	void SitEnd();
	void Overcomming();

	// Called when the game starts
	virtual void BeginPlay() override;
};
