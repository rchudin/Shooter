// Copyright © 2020 ruslanchudin.com


#include "FireWeapon.h"





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


void AFireWeapon::OnRep_Instigator()
{
    Super::OnRep_Instigator();

    ForgetGetViewPointLambda();
}


void AFireWeapon::RemoveUpdatingWidget()
{
    CurrentAmmo.UpdateWidget = nullptr;
    TotalAmmo.UpdateWidget = nullptr;
}


void AFireWeapon::Detach()
{
    Super::Detach();
    
    ForgetGetViewPointLambda();
}


void AFireWeapon::GetViewPoint(FVector& Out_Location, FVector& Out_Forward) const
{
    if (GetViewPointLambda)
    {
        GetViewPointLambda(Out_Location, Out_Forward);
    }
    else
    {
        Out_Location = GetMesh()->GetSocketLocation(MuzzleSocket);
        Out_Forward = GetMesh()->GetSocketRotation(MuzzleSocket).Vector();
    }
}


bool AFireWeapon::CanBeUsed() const
{
    if (CurrentAmmo <= 0) {
        return false;
    }

    return true;
}


void AFireWeapon::Use()
{
    if (CanBeUsed() && HasAuthority()) {
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


        
        if (GetNetMode() == NM_Standalone || 
            GetNetMode() == NM_ListenServer && GetLocalRole() == ROLE_Authority)
        {
            PlayUseEffects();
        }
    }
}


void AFireWeapon::PlayUseEffects()
{
    const FString LocalRoleEnumString = UEnum::GetValueAsString(GetLocalRole());
    UE_LOG(LogTemp, Log, TEXT("%s: %s  %s"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__), *LocalRoleEnumString);
    
    FHitResult OutHit;
    FVector Start;
    FVector End;
    CalculateTrajectory(Start, End);
    Trace(OutHit, Start, End);

    if (GetMesh())
    {
        const FVector NewStart = GetMesh()->GetSocketLocation(MuzzleSocket);
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
