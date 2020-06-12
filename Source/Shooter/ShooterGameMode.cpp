// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "GameFramework/HUD.h"
#include "ShooterPlayerController.h"
#include "ShooterPlayerState.h"
#include "UObject/ConstructorHelpers.h"

AShooterGameMode::AShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Shooter/Characters/UE4/BP_UnrealEngineCharacter"));
	static ConstructorHelpers::FClassFinder<AHUD> HudBPClass(TEXT("/Game/Shooter/UI/BP_InGameHud"));
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	if (HudBPClass.Class)
	{
		HUDClass = HudBPClass.Class;
	}

	PlayerControllerClass = AShooterPlayerController::StaticClass();
	PlayerStateClass = AShooterPlayerState::StaticClass();
}
