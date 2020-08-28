// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Shooter/UI/Widget/LoadingScreenInterface.h"
#include "Shooter/Core/UdpNetworking.h"
#include "ShooterGameInstance.generated.h"


UCLASS()
class SHOOTER_API UShooterGameInstance : public UGameInstance, public ILoadingScreenInterface
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    class UDataTable* WeaponInstanceData;

    UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

    UPROPERTY()
    class UUserWidget* LoadingScreenWidget;

    TSharedPtr<FUdpNetworking> UdpNetworking;

protected:
    virtual void Init() override;
    
    virtual void LoadComplete(const float LoadTime, const FString& MapName) override;

    virtual FGameInstancePIEResult StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer,
                                                                 const FGameInstancePIEParameters& Params) override;

public:
    UShooterGameInstance();

    virtual void AddLoadingScreenToViewport() override;

    virtual void RemoveLoadingScreenFromViewport() override;

    /** Returns Weapon Instance Data sub object **/
    FORCEINLINE UDataTable* GetWeaponInstanceData() const { return WeaponInstanceData; }
};
