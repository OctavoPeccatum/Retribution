// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/QuestGiver.h"
#include "Components/QuestGiverComponent.h"
#include "LooterShooter/LooterShooterCharacter.h"

// Sets default values
AQuestGiver::AQuestGiver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	QuestGiverComponent = CreateDefaultSubobject<UQuestGiverComponent>(TEXT("QuestGiverComponent"));
}

// Called when the game starts or when spawned
void AQuestGiver::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
}

void AQuestGiver::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AQuestGiver::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

FName AQuestGiver::Interact(ALooterShooterCharacter* PlayerCharacter)
{
	if (PlayerCharacter)
	{
		QuestGiverComponent->PerformQuest(PlayerCharacter);
	}

	return ID;
}


