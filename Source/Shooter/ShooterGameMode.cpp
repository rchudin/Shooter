// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "GameFramework/HUD.h"
#include "Character/ShooterPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AShooterGameMode::AShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Shooter/Characters/UE4/BP_UnrealEngineCharacter"));
	static ConstructorHelpers::FClassFinder<AHUD> HUD(TEXT("/Game/Shooter/UI/BP_InGameHud"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		HUDClass = HUD.Class;
		PlayerControllerClass = AShooterPlayerController::StaticClass();
	}
}
