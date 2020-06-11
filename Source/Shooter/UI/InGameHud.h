// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widget/AmmoWidget.h"
#include "InGameHud.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AInGameHud : public AHUD
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
            TSubclassOf<UUserWidget> CrosshairWidgetClass;

    UPROPERTY()
        class UUserWidget* CrosshairWidget;
    
    UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
        TSubclassOf<UAmmoWidget> AmmoWidgetClass;
    
    UPROPERTY()
        class UAmmoWidget* AmmoWidget;
public:
    AInGameHud();

    UFUNCTION()
        void UpdateCurrentAmmo(const int& Count) const;
    
    UFUNCTION()
        void UpdateTotalAmmo(const int& Count) const;

protected:
    virtual void BeginPlay() override;
    
    virtual void DrawHUD() override;

    
    
};
