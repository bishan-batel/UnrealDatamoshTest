// Copyright Epic Games, Inc. All Rights Reserved.

#include "DatamoshTestGameMode.h"
#include "DatamoshTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADatamoshTestGameMode::ADatamoshTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
