// Copyright © 2020 ruslanchudin.com


#include "MainMenuGameMode.h"
#include "Shooter/UI/MainMenuHUD.h"


AMainMenuGameMode::AMainMenuGameMode()
{
    static ConstructorHelpers::FClassFinder<AHUD> HudBPClass(TEXT("/Game/Shooter/UI/BP_MainMenuHUD"));
    if (HudBPClass.Class)
    {
        HUDClass = HudBPClass.Class;
    }
}
