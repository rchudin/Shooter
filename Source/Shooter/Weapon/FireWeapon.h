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

    UPROPERTY(EditDefaultsOnly, Category = Mesh)
        FName MuzzleSocket;

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

    virtual void Detach() override;
    
    virtual void RemoveUpdatingWidget() override;

    void SetGetViewPointLambda(TFunction<void (FVector&, FVector&)>  F) { GetViewPointLambda = F; }

    void SetOnCurrentAmmoUpdateFunction(TFunction<void(const int& Value)> F) {CurrentAmmo.UpdateWidget = F; CurrentAmmo.OnUpdate(); }
	
    void SetOnTotalAmmoUpdateFunction(TFunction<void(const int& Value)> F) {TotalAmmo.UpdateWidget = F; TotalAmmo.OnUpdate(); }

    void ForgetGetViewPointLambda() { GetViewPointLambda = nullptr; };
	
protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    TFunction<void (FVector&, FVector&)>  GetViewPointLambda;
    
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

    virtual void OnRep_Instigator() override;
    
    virtual void  RestoreToDefaultStats() override;

    virtual void GetViewPoint(FVector& Out_Location, FVector& Out_Forward) const override;
    
    virtual void Use() override;

    virtual  void PlayUseEffects() override;

    void Fire();
};
