// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "Character/ShooterCharacter.h"
#include "Character/ShooterPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AShooterGameMode::AShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Shooter/Characters/UE4/BP_UnrealEngineCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
		PlayerControllerClass = AShooterPlayerController::StaticClass();
	}
}
