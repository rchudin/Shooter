// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Shooter/Weapon/InteractPawnsWeaponInterface.h"
#include "ShooterGameMode.generated.h"

UCLASS(minimalapi)
class AShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	virtual void SetWeaponsPlayer(APawn* NewPlayerPawn);
	
public:
	AShooterGameMode();
};



