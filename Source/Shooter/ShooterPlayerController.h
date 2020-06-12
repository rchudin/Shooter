// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterGameInstance.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
    AShooterPlayerController();

    const UDataTable* GetWeaponInstanceData() const;
    
    void SetFirstPersonViewingAngle() const;
    
    void SetThirdPersonViewingAngle() const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void BeginSpectatingState() override;

    void LoadWeapon() const;
};
