// Copyright © 2020 ruslanchudin.com


#include "FireWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter/FunctionLibrary.h"



void AFireWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(AFireWeapon, TotalAmmo, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AFireWeapon, CurrentAmmo, COND_OwnerOnly);
    DOREPLIFETIME(AFireWeapon, Reloading);
}


AFireWeapon::AFireWeapon()
{
    MaxCurrentAmmo = 31;
    MaxTotalAmmo = 180;
    UsageTimePeriod = 0.5f;
    UseRange = 10000.f;
    ReloadTime =  1.7f;
    Damage = 20.f;
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


void AFireWeapon::OnRep_Reloading()
{
    if (Reloading)
    {
        PlayReloadingEffects();
    }
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
    if (CurrentAmmo <= 0) return false;

    if (Reloading) return false;

    return true;
}


void AFireWeapon::Use()
{
    if (CanBeUsed() && HasAuthority()) {
        LOG_INSTANCE(LogTemp, Log, HasAuthority(), TEXT("Use weapon"));
        
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
        FVector Start;
        FVector End;
        CalculateTrajectory(Start, End);
        FHitResult OutHit = Trace(Start, End);
        --CurrentAmmo;
        
        Scatter = !Scatter;

        UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), Damage, End -Start, OutHit, GetInstigatorController(), this, nullptr);
        
        if (GetNetMode() == NM_Standalone || 
            (GetNetMode() == NM_ListenServer && GetLocalRole() == ROLE_Authority))
        {
            PlayUseEffects();
        }
    }
}


void AFireWeapon::PlayUseEffects()
{
    const FString LocalRoleEnumString = UEnum::GetValueAsString(GetLocalRole());
    LOG_INSTANCE(LogTemp, Log, HasAuthority(), TEXT("Play Use Effects"));
    /*UE_LOG(LogTemp, Log, TEXT("%s: %s  %s"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__), *LocalRoleEnumString);*/
    
    FVector Start;
    FVector End;
    CalculateTrajectory(Start, End);
    FHitResult OutHit = Trace(Start, End);

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
        if (GetMesh() && FireAnimation) {
            GetMesh()->PlayAnimation(FireAnimation, false);
        }
    }
        
    DrawDebugFireLine(OutHit, Start, End);
}


// [Server] Reload
void AFireWeapon::Server_Reload_Implementation()
{
    Reload();
}


void AFireWeapon::Reload()
{
    UE_LOG(LogTemp, Log, TEXT("%s: %s"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
    
    UWorld* World = GetWorld();
    
    if (World && HasAuthority() && TotalAmmo > 0 && CurrentAmmo != MaxCurrentAmmo  &&
        (!ReloadingTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(ReloadingTimerHandle)))
        {
        
        const int AppendAmmo = (TotalAmmo < MaxCurrentAmmo ? TotalAmmo : MaxCurrentAmmo) - CurrentAmmo;
        TotalAmmo -= AppendAmmo;
        Reloading = true;
        FTimerDelegate TimerCallback;
        TimerCallback.BindLambda([&, AppendAmmo]
        {
            CurrentAmmo += AppendAmmo;
            Reloading = false;
        });
        World->GetTimerManager().SetTimer(ReloadingTimerHandle, TimerCallback, ReloadTime, false);
        if (GetNetMode() == NM_Standalone || 
               (GetNetMode() == NM_ListenServer && GetLocalRole() == ROLE_Authority))
        {
            PlayReloadingEffects();
        }
    }
}


void AFireWeapon::PlayReloadingEffects()
{
    if (GetMesh() && ReloadAnimation) GetMesh()->PlayAnimation(ReloadAnimation, false);

    ACharacter* Character = Cast<ACharacter>(GetInstigator());
    if (Character)
    {
        Character->PlayAnimMontage(CharacterReloadAnimMontage);
    }
}