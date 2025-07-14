// Copyright Epic Games, Inc. All Rights Reserved.

#include "CoopAventureGameMode.h"
#include "CoopAventureCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACoopAventureGameMode::ACoopAventureGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
