// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterGameMode.h"
#include "GameFramework/HUD.h"
#include "Shooter/ShooterPlayerController.h"
#include "Shooter/ShooterPlayerState.h"
#include "Shooter/Core/FunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"

AShooterGameMode::AShooterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Shooter/Characters/UE4/BP_UnrealEngineCharacter"));
	static ConstructorHelpers::FClassFinder<AHUD> HudBPClass(TEXT("/Game/Shooter/UI/BP_InGameHUD"));
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

void AShooterGameMode::RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot)
{
	Super::RestartPlayerAtPlayerStart(NewPlayer, StartSpot);

	if (NewPlayer) SetWeaponsPlayer(NewPlayer->GetPawn());
}

void AShooterGameMode::SetWeaponsPlayer(APawn* NewPlayerPawn)
{
	LOG_INSTANCE(LogTemp, Log, HasAuthority(), TEXT("%s"), TEXT(__FUNCTION__));
	IInteractPawnsWeaponInterface* InteractPawnsWeapon = Cast<IInteractPawnsWeaponInterface>(NewPlayerPawn);
	if (InteractPawnsWeapon)
	{
		const AShooterPlayerState* PState = NewPlayerPawn->GetPlayerState<AShooterPlayerState>();
		if (PState)
		{
			const FWeaponInstance* WeaponInstance = PState->GetDefaultMainWeapon();
			if (WeaponInstance)
			{
				const auto Location = NewPlayerPawn->GetActorLocation();
				AWeapon* NewWeapon = AWeapon::CreateWeapon(GetWorld(), WeaponInstance->Weapon, Location);
				NewWeapon->RecoverConsumables();
				if (NewWeapon) InteractPawnsWeapon->TakeWeapon(NewWeapon);
			}
		}
	}
}
