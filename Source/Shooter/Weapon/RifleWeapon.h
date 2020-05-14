// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "FireWeapon.h"
#include "RifleWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API ARifleWeapon : public AFireWeapon
{
	GENERATED_BODY()
public:
    // Sets default values for this actor's properties
    ARifleWeapon();
protected:
    virtual void Use() override;

 
};
