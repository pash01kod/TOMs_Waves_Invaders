// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves_InvadersGameMode.h"
#include "Waves_InvadersHUD.h"
#include "Waves_InvadersCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWaves_InvadersGameMode::AWaves_InvadersGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AWaves_InvadersHUD::StaticClass();
}
