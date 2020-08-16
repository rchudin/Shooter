// Copyright © 2020 ruslanchudin.com


#include "ShooterPlayerController.h"
#include "ShooterPlayerState.h"

AShooterPlayerController::AShooterPlayerController()
{
}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();

    SetFirstPersonViewingAngle();
}

void AShooterPlayerController::BeginSpectatingState()
{
    Super::BeginSpectatingState();

    LoadWeapon();
}

const UDataTable* AShooterPlayerController::GetWeaponInstanceData() const
{
    UE_LOG(LogTemp, Log, TEXT("%s: %s"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));

    const UShooterGameInstance* GameInstance = Cast<UShooterGameInstance>(GetGameInstance());
    return GameInstance->GetWeaponInstanceData();
}

void AShooterPlayerController::LoadWeapon() const
{
    if (HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("%s: %s"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));

        AShooterPlayerState* PState = GetPlayerState<AShooterPlayerState>();
        if (PState)
        {
            const UDataTable* WeaponInstanceData = GetWeaponInstanceData();
            if (WeaponInstanceData)
            {
                const FString ContextString(TEXT("Weapon Instance"));
                FWeaponInstance* WeaponInstance = WeaponInstanceData->FindRow<FWeaponInstance>(
                    FName(TEXT("AK74")), ContextString, true);
                if (WeaponInstance) PState->SetDefaultMainWeapon(WeaponInstance);
            }
        }
    }
}

void AShooterPlayerController::SetFirstPersonViewingAngle() const
{
    PlayerCameraManager->ViewPitchMax = 70.0f;
    PlayerCameraManager->ViewPitchMin = -80.0f;
}

void AShooterPlayerController::SetThirdPersonViewingAngle() const
{
    PlayerCameraManager->ViewPitchMax = 90.0f;
    PlayerCameraManager->ViewPitchMin = -90.0f;
}
