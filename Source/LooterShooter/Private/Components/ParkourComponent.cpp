// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ParkourComponent.h"
#include "LooterShooter/LooterShooterCharacter.h"

// Sets default values for this component's properties
UParkourComponent::UParkourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UParkourComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

