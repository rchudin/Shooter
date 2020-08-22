// Copyright © 2020 ruslanchudin.com


#include "WeaponManager.h"
#include "Shooter/Core/FunctionLibrary.h"


UWeaponManager::UWeaponManager()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicated(true);
}

void UWeaponManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UWeaponManager, CurrentWeapon);
    DOREPLIFETIME_CONDITION(UWeaponManager, MainWeapon, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(UWeaponManager, SecondWeapon, COND_OwnerOnly);
}

void UWeaponManager::OnRep_CurrentWeapon()
{
    AttachCurrentWeapon();
    SetFunctionGetViewPointInWeapon();
}

void UWeaponManager::CreateWidgets()
{
    if (GetNetMode() == NM_Standalone ||
        GetOwnerRole() == ROLE_AutonomousProxy)
    {
        if (CrosshairWidgetClass && !CrosshairWidget)
        {
            CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
            if (CrosshairWidget) CrosshairWidget->AddToViewport();
        }
        if (AmmoWidgetClass && !AmmoWidget)
        {
            AmmoWidget = CreateWidget<UAmmoWidget>(GetWorld(), AmmoWidgetClass);
            if (AmmoWidget)
            {
                AmmoWidget->AddToViewport();
                SetUpdatingWidgetInWeapon();
            }
        }
    }
}

void UWeaponManager::RemoveWidgets() const
{
    CurrentWeapon->RemoveUpdatingWidget();
    if (CrosshairWidget)
    {
        if (CrosshairWidget->IsInViewport()) CrosshairWidget->RemoveFromViewport();
        CrosshairWidget->Destruct();
    }
    if (AmmoWidget)
    {
        if (AmmoWidget->IsInViewport()) AmmoWidget->RemoveFromViewport();
        AmmoWidget->Destruct();
    }
}

void UWeaponManager::SetUpdatingWidgetInWeapon()
{
    auto FWeapon = Cast<AFireWeapon>(CurrentWeapon);
    if (CurrentWeapon && AmmoWidget)
    {
        FWeapon->SetFunctionUpdatingWidgetCurrentAmmo([&](const int& Count)
        {
            if (AmmoWidget) AmmoWidget->UpdateCurrentAmmo(Count);
        });
        FWeapon->SetFunctionUpdatingWidgetTotalAmmo([&](const int& Count)
        {
            if (AmmoWidget) AmmoWidget->UpdateTotalAmmo(Count);
        });
    }
}

void UWeaponManager::SetFunctionGetViewPointInWeapon() const
{
    if (CurrentWeapon) CurrentWeapon->SetFunctionGetViewPoint(GetViewPointLambda);
}

void UWeaponManager::SaveWeapon(AWeapon* Weapon)
{
    switch (Weapon->GetType())
    {
    case Second:
        if (SecondWeapon) SecondWeapon->Detach();
        SecondWeapon = Weapon;
        break;
    default:
        if (MainWeapon) MainWeapon->Detach();
        MainWeapon = Weapon;
        break;
    }
}

void UWeaponManager::AttachCurrentWeapon()
{
    if (CurrentWeapon && AttachWeaponLambda)
    {
        CurrentWeapon->GetMesh()->SetSimulatePhysics(false);
        const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
        AttachWeaponLambda(CurrentWeapon, Rules);

        SetUpdatingWidgetInWeapon();
        SetFunctionGetViewPointInWeapon();
    }
}

void UWeaponManager::TakeWeapon(AWeapon* Weapon)
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    LOG_INSTANCE(LogTemp, Log, Pawn->HasAuthority(), TEXT("%s"), TEXT(__FUNCTION__));
    if (Pawn->HasAuthority() && Weapon && AttachWeaponLambda)
    {
        SaveWeapon(Weapon);
        CurrentWeapon = Weapon;
        CurrentWeapon->SetInstigator(Pawn);
        CurrentWeapon->SetOwner(Pawn->GetController());

        AttachCurrentWeapon();
    }
}

void UWeaponManager::UseWeapon() const
{
    if (CurrentWeapon) CurrentWeapon->Server_Use();
}

void UWeaponManager::StopUseWeapon() const
{
    if (CurrentWeapon) CurrentWeapon->Server_StopUse();
}

void UWeaponManager::Reload() const
{
    if (CurrentWeapon)
    {
        auto FWeapon = Cast<AFireWeapon>(CurrentWeapon);
        FWeapon->Server_Reload();
    }
}

void UWeaponManager::Server_DropCurrentWeapon_Implementation() const
{
    DropCurrentWeapon();
}

void UWeaponManager::DropCurrentWeapon() const
{
    if (CurrentWeapon) CurrentWeapon->Detach();
}
