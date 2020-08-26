// Copyright © 2020 ruslanchudin.com


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Shooter/UI/Widget/LoadingScreenInterface.h"


UMainMenuWidget::UMainMenuWidget(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

bool UMainMenuWidget::Initialize()
{
    const bool Success = Super::Initialize();
    if (!Success) return false;

    if (!ensure(BTNNewGame != nullptr)) return false;
    BTNNewGame->OnClicked.AddDynamic(this, &UMainMenuWidget::NewGameButtonClicked);
    return true;
}

void UMainMenuWidget::NewGameButtonClicked()
{
    if (!NewGameMap.IsNull())
    {
        ILoadingScreenInterface* LoadingScreen = Cast<ILoadingScreenInterface>(GetGameInstance());
        if (LoadingScreen) LoadingScreen->AddLoadingScreenToViewport();

        FTimerHandle Handle;
        GetWorld()->GetTimerManager().SetTimer(Handle, [this]()
        {
            UE_LOG(LogTemp, Log, TEXT("Open map '%s'"), *NewGameMap.GetAssetName());
            UGameplayStatics::OpenLevel(GetWorld(), *NewGameMap.GetAssetName());
        }, 1.5f, false);
    }
}
