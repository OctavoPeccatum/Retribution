// Copyright Epic Games, Inc. All Rights Reserved.

#include "LooterShooterGameMode.h"
#include "LooterShooterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALooterShooterGameMode::ALooterShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
