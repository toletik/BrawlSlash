// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BrawlSlashGameMode.h"
#include "UObject/ConstructorHelpers.h"

ABrawlSlashGameMode::ABrawlSlashGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Character/Player/MyCharacter_Player"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
