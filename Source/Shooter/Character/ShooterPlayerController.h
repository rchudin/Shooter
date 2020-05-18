// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
    
    void SetFirstPersonViewingAngle() const;
    
    void SetThirdPersonViewingAngle() const;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
