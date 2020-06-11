// Copyright © 2020 ruslanchudin.com


#include "AmmoWidget.h"

UAmmoWidget::UAmmoWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    
}

void UAmmoWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
}


void UAmmoWidget::UpdateCurrentAmmo(const int& Count) const
{
    if (TXTCurrentAmmo)
    {
        if (TXTCurrentAmmo->Visibility ==ESlateVisibility::Hidden)
        {
            TXTCurrentAmmo->SetVisibility(ESlateVisibility::Visible);
        }

        TXTCurrentAmmo->SetText(FText::FromString(FString::FromInt(Count)));
    }
}

void UAmmoWidget::UpdateTotalAmmo(const int& Count) const
{
    if (TXTTotalAmmo)
    {
        if(TXTTotalAmmo->Visibility ==ESlateVisibility::Hidden)
        {
            TXTTotalAmmo->SetVisibility(ESlateVisibility::Visible);
        }

        TXTTotalAmmo->SetText(FText::FromString(FString::FromInt(Count)));
    }
}

