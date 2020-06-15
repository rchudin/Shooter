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

    UPROPERTY(EditDefaultsOnly, Category = Animation) 
        class UAnimationAsset* FireAnimation;

    UPROPERTY(EditDefaultsOnly, Category = Animation) 
        class UAnimationAsset* ReloadAnimation;

    UFUNCTION()
        void OnRep_CurrentAmmo() const { CurrentAmmo.OnUpdate(); }
	
    UFUNCTION()
        void OnRep_TotalAmmo() const { TotalAmmo.OnUpdate(); }
public:
    AFireWeapon();
    
    /** Projectile class to spawn */
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class APawn> Projectile;

    virtual bool CanBeUsed() const override;
    
    virtual void RemoveUpdatingWidget() override;

    void SetOnCurrentAmmoUpdateFunction(TFunction<void(const int& Value)> F) {CurrentAmmo.UpdateWidget = F; CurrentAmmo.OnUpdate(); }
	
    void SetOnTotalAmmoUpdateFunction(TFunction<void(const int& Value)> F) {TotalAmmo.UpdateWidget = F; TotalAmmo.OnUpdate(); }
	
protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UPROPERTY(EditAnywhere, Category = Stats)
        bool AutoFire;
    
    UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_CurrentAmmo, Category = Stats, meta = (AllowPrivateAccess = "true"))
        FCounter CurrentAmmo;
	
    UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_TotalAmmo, Category = Stats,  meta = (AllowPrivateAccess = "true"))
        FCounter TotalAmmo;

    UPROPERTY(EditAnywhere, Category = Stats)
        int MaxCurrentAmmo;
	
    UPROPERTY(EditAnywhere, Category = Stats)
        int MaxTotalAmmo;

    virtual void  RestoreToDefaultStats() override;
    
    virtual void Use() override;

    virtual  void PlayUseEffects() override;

    void Fire();
};
