// Copyright © 2020 ruslanchudin.com


#include "ShooterGameInstance.h"
#include "Blueprint/UserWidget.h"


UShooterGameInstance::UShooterGameInstance()
{
    static ConstructorHelpers::FObjectFinder<UDataTable>
        WeaponInstanceObject(TEXT("/Game/Shooter/Weapon/DT_Weapon"));
    if (WeaponInstanceObject.Succeeded()) WeaponInstanceData = WeaponInstanceObject.Object;

    static ConstructorHelpers::FClassFinder<UUserWidget>
        LoadingScreenBPClass(TEXT("/Game/Shooter/UI/WBP_LoadingScreen"));
    if (LoadingScreenBPClass.Class) LoadingScreenWidgetClass = LoadingScreenBPClass.Class;
}

void UShooterGameInstance::Init()
{
    Super::Init();
    const auto Services = new FGameServices(GameServicesAddress, GameServicesPort);
    EGameServicesStatus::Success == *Services ? GameServices = MakeShareable(Services) : delete Services;
}

void UShooterGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
    Super::LoadComplete(LoadTime, MapName);

    RemoveLoadingScreenFromViewport();
}

void UShooterGameInstance::AddLoadingScreenToViewport()
{
    if (!IsDedicatedServerInstance() && LoadingScreenWidgetClass)
    {
        LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenWidgetClass);
        if (LoadingScreenWidget) LoadingScreenWidget->AddToViewport();
    }
}

void UShooterGameInstance::RemoveLoadingScreenFromViewport()
{
    if (LoadingScreenWidget)
    {
        if (LoadingScreenWidget->IsInViewport()) LoadingScreenWidget->RemoveFromViewport();
        LoadingScreenWidget->Destruct();
        if (LoadingScreenWidget) LoadingScreenWidget = nullptr;
    }
}
