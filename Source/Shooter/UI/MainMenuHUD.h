// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widget/MainMenuWidget.h"
#include "MainMenuHUD.generated.h"


UCLASS()
class SHOOTER_API AMainMenuHUD : public AHUD
{
    GENERATED_BODY()

    TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

    UPROPERTY()
    class UMainMenuWidget* MainMenuWidget;

protected:
    virtual void BeginPlay() override;

public:
    AMainMenuHUD();
};
