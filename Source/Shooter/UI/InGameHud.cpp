// Copyright © 2020 ruslanchudin.com


#include "InGameHud.h"


AInGameHud::AInGameHud()
{
    
}

void AInGameHud::BeginPlay()
{
    Super::BeginPlay();

    if(CrosshairWidgetClass)
    {
        CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
        if (CrosshairWidget)
        {
            CrosshairWidget->AddToViewport();
        }
    }
    

    if (AmmoWidgetClass)
    {
        AmmoWidget = CreateWidget<UAmmoWidget>(GetWorld(), AmmoWidgetClass);
        if (AmmoWidget)
        {
            AmmoWidget->AddToViewport();
        }
    }
}

void AInGameHud::DrawHUD()
{
    Super::DrawHUD();
    
}

void AInGameHud::UpdateCurrentAmmo(const int& Count) const
{
    if (AmmoWidget)
    {
        AmmoWidget->UpdateCurrentAmmo(Count);
    }
}

void AInGameHud::UpdateTotalAmmo(const int& Count) const
{
    if (AmmoWidget)
    {
        AmmoWidget->UpdateTotalAmmo(Count);
    }
}
