// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Shooter/Core/Counter.h"
#include "FireWeapon.generated.h"


UCLASS()
class SHOOTER_API AFireWeapon : public AWeapon
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
    class UAnimationAsset* FireAnimation;

    UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
    class UAnimationAsset* ReloadAnimation;

    UPROPERTY(EditDefaultsOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
    class UAnimMontage* CharacterReloadAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
    FName MuzzleSocket;

    UPROPERTY(VisibleDefaultsOnly, Transient, ReplicatedUsing = OnRep_Scatter,
        Category = Fire, meta = (AllowPrivateAccess = "true"))
    bool Scatter;

    UPROPERTY(VisibleDefaultsOnly, Transient, ReplicatedUsing = OnRep_IsReloading,
        Category = Animation, meta = (AllowPrivateAccess = "true"))
    bool IsReloading;

    UFUNCTION()
    void OnRep_CurrentAmmo() const { CurrentAmmo.OnUpdate(); }

    UFUNCTION()
    void OnRep_TotalAmmo() const { TotalAmmo.OnUpdate(); }

    UFUNCTION()
    void OnRep_Scatter() const;

    UFUNCTION()
    void OnRep_IsReloading() const;


protected:
    UPROPERTY(EditDefaultsOnly, Category = Stats)
    int MaxCurrentAmmo;

    UPROPERTY(VisibleDefaultsOnly, Transient, ReplicatedUsing = OnRep_CurrentAmmo,
        Category = Stats, meta = (AllowPrivateAccess = "true"))
    FCounterInt CurrentAmmo;

    UPROPERTY(EditDefaultsOnly, Category = Stats)
    int DefaultTotalAmmo;

    UPROPERTY(VisibleDefaultsOnly, Transient, ReplicatedUsing = OnRep_TotalAmmo,
        Category = Stats, meta = (AllowPrivateAccess = "true"))
    FCounterInt TotalAmmo;

    UPROPERTY(EditDefaultsOnly, Category = Stats)
    float ReloadTime;

    UPROPERTY(EditDefaultsOnly, Category = Stats)
    bool AutoFire = false;

    UPROPERTY(EditDefaultsOnly, Category = Stats)
    float UsageInterval;

    UPROPERTY(EditDefaultsOnly, Category = Stats)
    float UsedRange;

    UPROPERTY()
    FTimerHandle UseTimerHandle;

    UPROPERTY()
    FTimerHandle ReloadingTimerHandle;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    FHitResult CalculateTrajectory(FVector* Start, FVector* Forward, FVector* End) const;

    virtual void Use() override;

    void Fire();

    void PlayUseEffects() const;

    void Reload();

    void PlayReloadingEffects() const;

public:
    AFireWeapon();

    virtual void RemoveUpdatingWidget() override;

    bool CanUseFireWeapon() const;

    virtual void RecoverConsumables() override;

    // [Server] Reload
    UFUNCTION(Reliable, Server)
    void Server_Reload();

    FORCEINLINE void SetFunctionUpdatingWidgetCurrentAmmo(TFunction<void(const int& Value)> Function);

    FORCEINLINE void SetFunctionUpdatingWidgetTotalAmmo(TFunction<void(const int& Value)> Function);
};
