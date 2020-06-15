// Copyright © 2020 ruslanchudin.com


#include "FireWeapon.h"
#include "DrawDebugHelpers.h"
#include "GameModeInfoCustomizer.h"




void AFireWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(AFireWeapon, TotalAmmo, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AFireWeapon, CurrentAmmo, COND_OwnerOnly);
}


AFireWeapon::AFireWeapon()
{
    MaxCurrentAmmo = 31;
    MaxTotalAmmo = 180;
    UsageTimePeriod = 0.5f;
    UseRange = 10000.f;
}


void AFireWeapon::RestoreToDefaultStats()
{
    Super::RestoreToDefaultStats();
    
    CurrentAmmo = MaxCurrentAmmo;
    TotalAmmo = MaxTotalAmmo;
}


bool AFireWeapon::CanBeUsed() const
{
    if (CurrentAmmo <= 0) {
        return false;
    }

    return true;
}


void AFireWeapon::RemoveUpdatingWidget()
{
    CurrentAmmo.UpdateWidget = nullptr;
    TotalAmmo.UpdateWidget = nullptr;
}


void AFireWeapon::Use()
{
    if (CanBeUsed()) {
        UE_LOG(LogTemp, Log, TEXT("%s: Use Weapon"), HasAuthority()?TEXT("Server"):TEXT("Client"));
        
        UWorld* World = GetWorld();
        if (World) {
            FTimerDelegate TimerCallback;
            TimerCallback.BindLambda([&, World]
            {
                if (CanBeUsed() && AutoFire && Pressed)
                {
                    Fire();
                }
                else
                {
                    World->GetTimerManager().ClearTimer(UsagePeriodTimerHandle);
                }
            });
            Fire();
            World->GetTimerManager().SetTimer(UsagePeriodTimerHandle, TimerCallback, UsageTimePeriod, true);
        }

    }
}

void AFireWeapon::Fire()
{
    if (HasAuthority())
    {
        FHitResult OutHit;
        FVector Start;
        FVector End;
        CalculateTrajectory(Start, End);
        Trace(OutHit, Start, End);
        --CurrentAmmo;
        
        Scatter = !Scatter;

        if (GetNetMode() == NM_Standalone || GetLocalRole() == ROLE_Authority)
        {
            PlayUseEffects();
        }
    }
}


void AFireWeapon::PlayUseEffects()
{
    FHitResult OutHit;
    FVector Start;
    FVector End;
    CalculateTrajectory(Start, End);
    Trace(OutHit, Start, End);

    Trace(OutHit, Start, End);
    if (GetMesh())
    {
        const FVector NewStart = GetMesh()->GetSocketLocation("skt_muzzle");
        if (!NewStart.IsZero())
        {
            Start = NewStart;
            /*if (Projectile)
            {
                FRotator Rotate =  UKismetMathLibrary::FindLookAtRotation(Start, OutHit.Location);
            }*/
        }
        if (FireAnimation) {
            GetMesh()->PlayAnimation(FireAnimation, false);
        }
    }
        
    DrawDebugFireLine(OutHit, Start, End);
}
