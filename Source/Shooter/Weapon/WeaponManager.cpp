// Copyright © 2020 ruslanchudin.com


#include "WeaponManager.h"

// Sets default values for this component's properties
UWeaponManager::UWeaponManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}



void UWeaponManager::SetupAttachment(class AShooterCharacter* InParent)
{
	ParrentCharacter = InParent;
}

void UWeaponManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	/*Super::GetLifetimeReplicatedProps(OutLifetimeProps); */
	
	DOREPLIFETIME_CONDITION(UWeaponManager, MainWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UWeaponManager, SecondaryWeapon, COND_OwnerOnly);
}
