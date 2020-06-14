// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHud.generated.h"


UCLASS()
class SHOOTER_API AInGameHud : public AHUD
{
	GENERATED_BODY()
public:
    AInGameHud();

protected:
    virtual void BeginPlay() override;
    
    virtual void DrawHUD() override;

    
    
};
