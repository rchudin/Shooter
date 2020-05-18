// Copyright © 2020 ruslanchudin.com


#include "ShooterPlayerController.h"

AShooterPlayerController::AShooterPlayerController()
{
}

void AShooterPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    SetFirstPersonViewingAngle();
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
