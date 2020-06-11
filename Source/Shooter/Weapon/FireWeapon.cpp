// Copyright © 2020 ruslanchudin.com


#include "FireWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameModeInfoCustomizer.h"
#include "Math/Rotator.h"
#include "Kismet/KismetMathLibrary.h"


AFireWeapon::AFireWeapon()
{
    UseRange = 10000.f;
}

void AFireWeapon::Use()
{
    if (CanBeUsed()) {

        UE_LOG(LogTemp, Log, TEXT("%s: Use Weapon"), HasAuthority()?TEXT("Server"):TEXT("Client"));
        
        UWorld* World = GetWorld();
        if (World) {
            FTimerDelegate TimerCallback;
            TimerCallback.BindLambda([this,World]
            {
                World->GetTimerManager().ClearTimer(FireTimerHandle);
            });
            World->GetTimerManager().SetTimer(FireTimerHandle, TimerCallback, UseRate, false);

            Fire();
        }

    }
}

void AFireWeapon::Fire()
{
    FHitResult OutHit;
    FVector Start;
    FVector End;
    if (GetTrajectory(Start, End))
    {
        AddCurrentAmmo(-1);
        Trace(OutHit, Start, End);
        MulticastUseEffects();
    }
}


void AFireWeapon::UseEffects()
{
    FHitResult OutHit;
    FVector Start;
    FVector End;
    if (GetTrajectory(Start, End))
    {
        Trace(OutHit, Start, End);
        if (GetMesh())
        {
            const FVector NewStart = GetMesh()->GetSocketLocation("skt_muzzle");
            if (!NewStart.IsZero())
            {
                Start = NewStart;
                if (Projectile)
                {
                    FRotator Rotate =  UKismetMathLibrary::FindLookAtRotation(Start, OutHit.Location);
                }
            }
            if (FireAnimation) {
                GetMesh()->PlayAnimation(FireAnimation, false);
            }
        }
        
        DrawDebugFireLine(OutHit, Start, End);
    }
}
