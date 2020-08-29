// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireWeapon.h"
#include "Shooter/UI/Widget/AmmoWidget.h"
#include "InteractPawnsWeaponInterface.h"
#include "WeaponManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UWeaponManager : public UActorComponent, public IInteractPawnsWeaponInterface
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

    UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_CurrentWeapon, Category = "Weapon")
    class AWeapon* CurrentWeapon;

    UPROPERTY(replicated)
    class AWeapon* MainWeapon;

    UPROPERTY(replicated)
    class AWeapon* SecondWeapon;

    TFunction<void(AActor*, const FAttachmentTransformRules&)> AttachWeaponLambda;

    UFUNCTION()
    void OnRep_CurrentWeapon();

protected:
    TFunction<void (FVector*, FVector*)> GetViewPointLambda;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    void SetUpdatingWidgetInWeapon();

    void SetFunctionGetViewPointInWeapon() const;

    void SaveWeapon(AWeapon* Weapon);

    void AttachCurrentWeapon();

    // [Server] DropCurrentWeapon
    UFUNCTION(Reliable, Server)
    void Server_DropCurrentWeapon() const;
    
    void DropCurrentWeapon() const;
    
public:
    UWeaponManager();

    void CreateWidgets();

    void RemoveWidgets() const;

    virtual bool TakeWeapon(AWeapon* Weapon) override;

    void UseWeapon() const;

    void StopUseWeapon() const;

    void Reload() const;

    FORCEINLINE void DropWeapon() const { if (CurrentWeapon) Server_DropCurrentWeapon(); }

    FORCEINLINE void SetFunctionAttachedWeapon(
        const TFunction<void(AActor*, const FAttachmentTransformRules&)> Fn)
    {
        AttachWeaponLambda = Fn;
    }

    FORCEINLINE void SetFunctionGetViewPoint(const TFunction<void (FVector*, FVector*)> Fn) { GetViewPointLambda = Fn; }

    FORCEINLINE const class AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};
