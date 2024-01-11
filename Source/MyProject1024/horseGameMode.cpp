// Copyright Epic Games, Inc. All Rights Reserved.

#include "horseGameMode.h"
#include "horseCharacter.h"
#include "horse_spline_view.h"
#include "UObject/ConstructorHelpers.h"

AhorseGameMode::AhorseGameMode()
{
	// set default pawn class to our Blueprinted character
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/horse/bp/horse_bp"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}*/
}
