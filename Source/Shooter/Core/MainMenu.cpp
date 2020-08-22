// Copyright © 2020 ruslanchudin.com


#include "MainMenu.h"

#define NEW_GAME_MAP "Desert"

void MainMenu::StartNewGame(const UObject* WorldContextObject)
{
    UGameplayStatics::OpenLevel(WorldContextObject, NEW_GAME_MAP);
}
