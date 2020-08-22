// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"


UCLASS()
class SHOOTER_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
    class UButton* BTNNewGame;

protected:
    virtual void NativeConstruct() override;

    virtual bool Initialize() override;

    UFUNCTION()
    void NewGameButtonClicked();

public:
    UMainMenuWidget(const FObjectInitializer& ObjectInitializer);
};
