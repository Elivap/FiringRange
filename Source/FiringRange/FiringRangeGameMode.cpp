// Copyright Epic Games, Inc. All Rights Reserved.

#include "FiringRangeGameMode.h"
#include "FiringRangeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFiringRangeGameMode::AFiringRangeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
