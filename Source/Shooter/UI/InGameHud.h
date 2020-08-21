// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"


UCLASS()
class SHOOTER_API AInGameHUD : public AHUD
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

    virtual void DrawHUD() override;

public:
    AInGameHUD();
};
