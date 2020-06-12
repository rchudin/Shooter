// Copyright © 2020 ruslanchudin.com


#include "ShooterGameInstance.h"



UShooterGameInstance::UShooterGameInstance()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> WeaponInstanceObject(TEXT("/Game/Shooter/Weapon/DT_Weapon"));
    if (WeaponInstanceObject.Succeeded())
    {
        WeaponInstanceData = WeaponInstanceObject.Object;
    }
}
