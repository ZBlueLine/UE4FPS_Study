// Copyright Epic Games, Inc. All Rights Reserved.

#include "demoGameMode.h"
#include "demoHUD.h"
#include "demoCharacter.h"
#include "UObject/ConstructorHelpers.h"

AdemoGameMode::AdemoGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AdemoHUD::StaticClass();
}
