// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpaceAdventure3DGameMode.h"
#include "SpaceAdventure3DPawn.h"

ASpaceAdventure3DGameMode::ASpaceAdventure3DGameMode()
{
	// set default pawn class to our flying pawn
	DefaultPawnClass = ASpaceAdventure3DPawn::StaticClass();
}
