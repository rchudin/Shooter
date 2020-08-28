// Copyright © 2020 ruslanchudin.com


#include "ShooterGameInstance.h"


#include "SocketSubsystem.h"
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

    UdpNetworking = MakeShareable(new FUdpNetworking);
    auto RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    bool bIsValid;
    RemoteAddress->SetIp(TEXT("127.0.0.1"), bIsValid);
    RemoteAddress->SetPort(6000);
    if (bIsValid)
    {
        UdpNetworking->SendMessage(FString("Hello world! =} #@22@#"), *RemoteAddress);
    }
}

void UShooterGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
    Super::LoadComplete(LoadTime, MapName);

    RemoveLoadingScreenFromViewport();
}

FGameInstancePIEResult UShooterGameInstance::StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer,
    const FGameInstancePIEParameters& Params)
{
    return  Super::StartPlayInEditorGameInstance(LocalPlayer, Params);
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
