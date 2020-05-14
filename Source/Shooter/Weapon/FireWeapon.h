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
protected:
    virtual void UseWeapon() override;
};
