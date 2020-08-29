// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Shooter/Weapon/Weapon.h"
#include "InteractPawnsWeaponInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractPawnsWeaponInterface : public UInterface
{
    GENERATED_BODY()
};


class SHOOTER_API IInteractPawnsWeaponInterface
{
    GENERATED_BODY()

public:
	virtual bool TakeWeapon(AWeapon * Weapon)
    {
        check(0 && "You must override this");
		return false;
	
    }
};
