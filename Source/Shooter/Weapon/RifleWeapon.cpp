// Copyright © 2020 ruslanchudin.com


#include "RifleWeapon.h"

ARifleWeapon::ARifleWeapon()
{
    UseRate = 0.1f;
}

void ARifleWeapon::Use()
{
    if (CanBeUsed()) {
        UE_LOG(LogTemp, Log, TEXT("Use Weapon Rifle"));
        
        UWorld* World = GetWorld();
        if (World) {
            FTimerDelegate TimerCallback;
            TimerCallback.BindLambda([this,World]
            {
                if (FireTimerExpired)
                {
                    World->GetTimerManager().ClearTimer(FireTimerHandle);
                }else{
                    MulticastUseEffects();
                }
            });
            
            TimerCallback.Execute();
            
            World->GetTimerManager().SetTimer(FireTimerHandle, TimerCallback, UseRate, true);

            
        }
    }
}
