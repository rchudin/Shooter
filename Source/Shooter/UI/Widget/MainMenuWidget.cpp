// Copyright © 2020 ruslanchudin.com


#include "MainMenuWidget.h"
#include "Shooter/Core/Auth.h"


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
    Auth::StartNewGame(GetWorld());
}
