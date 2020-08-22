// Copyright © 2020 ruslanchudin.com


#include "HealthWidget.h"



UHealthWidget::UHealthWidget(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
    
}

void UHealthWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHealthWidget::UpdateHealth(const float &Count, const float &Percent) const 
{
    UE_LOG(LogTemp, Log, TEXT("Count: %f Percent: %f"), Count, Percent);
    if (Count >= 0 && Percent >= 0 && Percent <= 1)
    {
        if (TXTHealth)
        {
            if (Percent < 1)
            {
                if(TXTHealth->Visibility == ESlateVisibility::Hidden)
                {
                    TXTHealth->SetVisibility(ESlateVisibility::Visible);
                }
            }
            else
            {
                if(TXTHealth->Visibility == ESlateVisibility::Visible)
                {
                    TXTHealth->SetVisibility(ESlateVisibility::Hidden);
                }
            }
            
            TXTHealth->SetText(FText::FromString(FString::FromInt(Count)));
        }

        if (PBHealth)
        {
            if (Percent < 1)
            {
                if(PBHealth->Visibility == ESlateVisibility::Hidden)
                {
                    PBHealth->SetVisibility(ESlateVisibility::Visible);
                }
            }
            else
            {
                if(PBHealth->Visibility == ESlateVisibility::Visible)
                {
                    PBHealth->SetVisibility(ESlateVisibility::Hidden);
                }
            }

            if (Percent < 0.5f)
            {
                PBHealth->SetFillColorAndOpacity(FLinearColor(255,0,0));
            }
            else
            {
                PBHealth->SetFillColorAndOpacity(FLinearColor(255,255,255));
            }
            
            PBHealth->SetPercent(Percent);
        }
    }
}
