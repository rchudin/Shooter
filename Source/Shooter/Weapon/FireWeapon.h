// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "FireWeapon.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTER_API AFireWeapon : public AWeapon
{
	GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, Category = Animation) 
        class UAnimationAsset* FireAnimation;

    UPROPERTY(EditDefaultsOnly, Category = Animation) 
        class UAnimationAsset* ReloadAnimation;
    
protected:
    virtual void Use() override;

    virtual void UseEffects() override;
};
