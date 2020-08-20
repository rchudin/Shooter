// Copyright © 2020 ruslanchudin.com


#include "FireWeapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter/FunctionLibrary.h"


AFireWeapon::AFireWeapon()
{
    MaxCurrentAmmo = 31;
    DefaultTotalAmmo = MaxCurrentAmmo * 4;
    ReloadTime = 1.f;
    UsageInterval = 0.2f;
    UsedRange = 10000.f;
    Damage = 20.f;
}

void AFireWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(AFireWeapon, TotalAmmo, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AFireWeapon, CurrentAmmo, COND_OwnerOnly);
    DOREPLIFETIME(AFireWeapon, Scatter);
    DOREPLIFETIME(AFireWeapon, IsReloading);
}

void AFireWeapon::OnRep_Scatter() const
{
    PlayUseEffects();
}

void AFireWeapon::OnRep_IsReloading() const
{
    PlayReloadingEffects();
}

void AFireWeapon::SetFunctionUpdatingWidgetCurrentAmmo(const TFunction<void(const int& Value)> Function)
{
    CurrentAmmo.UpdateWidget = Function;
    CurrentAmmo.OnUpdate();
}

void AFireWeapon::SetFunctionUpdatingWidgetTotalAmmo(const TFunction<void(const int& Value)> Function)
{
    TotalAmmo.UpdateWidget = Function;
    TotalAmmo.OnUpdate();
}

void AFireWeapon::RemoveUpdatingWidget()
{
    SetFunctionUpdatingWidgetCurrentAmmo(nullptr);
    SetFunctionUpdatingWidgetTotalAmmo(nullptr);
}

void AFireWeapon::RecoverConsumables()
{
    CurrentAmmo = MaxCurrentAmmo;
    TotalAmmo = DefaultTotalAmmo;
}

bool AFireWeapon::CanUseFireWeapon() const
{
    return CanUseWeapon() &&
        CurrentAmmo > 0 && UsedRange > 0 &&
        (!ReloadingTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(ReloadingTimerHandle));
}

void AFireWeapon::Use()
{
    if (CanUseFireWeapon() && (!UseTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(UseTimerHandle)))
    {
        LOG_INSTANCE(LogTemp, Log, HasAuthority(), TEXT("Use weapon"));

        FTimerDelegate TimerCallback;
        TimerCallback.BindLambda([&]
        {
            if (CanUseFireWeapon() && AutoFire && IsPressedUse)
            {
                Fire();
            }
            else
            {
                GetWorld()->GetTimerManager().ClearTimer(UseTimerHandle);
            }
        });
        Fire();
        GetWorld()->GetTimerManager().SetTimer(UseTimerHandle, TimerCallback, UsageInterval, true);
    }
}

FHitResult AFireWeapon::CalculateTrajectory(FVector* Start, FVector* Forward, FVector* End) const
{
    if (Start && Forward && End)
    {
        if (GetViewPointLambda) GetViewPointLambda(Start, Forward);
        *End = ((*Forward * UsedRange) + *Start);
        return Trace(*Start, *End);
    }
    return FHitResult();
}

void AFireWeapon::Fire()
{
    FVector Start, Forward, End;
    const FHitResult OutHit = CalculateTrajectory(&Start, &Forward, &End);
    --CurrentAmmo;
    Scatter = !Scatter;

    UGameplayStatics::ApplyPointDamage(OutHit.GetActor(), Damage, End - Start,
                                       OutHit, GetInstigatorController(), this, nullptr);

    if (GetNetMode() == NM_Standalone ||
        (GetNetMode() == NM_ListenServer && GetLocalRole() == ROLE_Authority))
    {
        PlayUseEffects();
    }
}

void AFireWeapon::PlayUseEffects() const
{
    FVector Start, Forward, End;
    const FHitResult OutHit = CalculateTrajectory(&Start, &Forward, &End);
    if (GetMesh())
    {
        const FVector NewStart = GetMesh()->GetSocketLocation(MuzzleSocket);
        if (!NewStart.IsZero())
        {
            Start = NewStart;
            // if (Projectile) FRotator Rotate = UKismetMathLibrary::FindLookAtRotation(Start, OutHit.Location);
        }
        if (GetMesh() && FireAnimation)
        {
            GetMesh()->PlayAnimation(FireAnimation, false);
        }
    }
    DrawDebugTraceLine(OutHit, Start, End);
}

// [Server] Reload
void AFireWeapon::Server_Reload_Implementation()
{
    Reload();
}

void AFireWeapon::Reload()
{
    UWorld* World = GetWorld();

    if (World && HasAuthority() && TotalAmmo > 0 && CurrentAmmo != MaxCurrentAmmo &&
        (!ReloadingTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(ReloadingTimerHandle)))
    {
        const int Lacks = MaxCurrentAmmo - CurrentAmmo;
        const int AppendAmmo = TotalAmmo < Lacks ? TotalAmmo : Lacks;
        TotalAmmo -= AppendAmmo;
        IsReloading = true;
        FTimerDelegate TimerCallback;
        TimerCallback.BindLambda([&, AppendAmmo]
        {
            CurrentAmmo += AppendAmmo;
            IsReloading = false;
        });
        World->GetTimerManager().SetTimer(ReloadingTimerHandle, TimerCallback, ReloadTime, false);
        if (GetNetMode() == NM_Standalone ||
            (GetNetMode() == NM_ListenServer && GetLocalRole() == ROLE_Authority))
        {
            PlayReloadingEffects();
        }
    }
}

void AFireWeapon::PlayReloadingEffects() const
{
    // if (GetMesh() && ReloadAnimation) GetMesh()->PlayAnimation(ReloadAnimation, false);

    ACharacter* Character = Cast<ACharacter>(GetInstigator());
    if (Character)
    {
        Character->PlayAnimMontage(CharacterReloadAnimMontage);
    }
}
