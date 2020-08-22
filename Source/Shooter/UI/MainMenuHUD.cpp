// Copyright © 2020 ruslanchudin.com


#include "MainMenuHUD.h"


AMainMenuHUD::AMainMenuHUD()
{
    static ConstructorHelpers::FClassFinder<UMainMenuWidget>
        MainMenuWidgetClassHelper(TEXT("/Game/Shooter/UI/BP_MainMenuWidget"));

    if (MainMenuWidgetClassHelper.Class)
    {
        MainMenuWidgetClass = MainMenuWidgetClassHelper.Class;
    }
}

void AMainMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuWidgetClass && !MainMenuWidget)
    {
        MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
    }
    if (MainMenuWidget) MainMenuWidget->AddToViewport();

    APlayerController* PC = Cast<APlayerController>(GetOwningPlayerController());
    if(PC)
    {
        PC->bShowMouseCursor = true; 
        PC->bEnableClickEvents = true; 
        PC->bEnableMouseOverEvents = true;
    }
    
}
