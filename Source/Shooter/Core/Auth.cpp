// Copyright © 2020 ruslanchudin.com


#include "Auth.h"
#include "Kismet/GameplayStatics.h"


#define NEW_GAME_MAP "Desert"


void Auth::StartNewGame(const UObject* WorldContextObject)
{
    UGameplayStatics::OpenLevel(WorldContextObject, NEW_GAME_MAP);
}
