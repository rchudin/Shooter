// Copyright © 2020 ruslanchudin.com


#include "WeaponManager.h"

// Sets default values for this component's properties
UWeaponManager::UWeaponManager()
{
	PrimaryComponentTick.bCanEverTick = true;
	/*SetNetAddressable();*/ // Make DSO components net addressable
	SetIsReplicated(true);
}

void UWeaponManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME(UWeaponManager, CurrentWeapon);
	DOREPLIFETIME_CONDITION(UWeaponManager, MainWeapon, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(UWeaponManager, SecondaryWeapon, COND_OwnerOnly);
}

void UWeaponManager::AttachWeapon() const
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());
	if (Character && CurrentWeapon && (!GetOwner()->HasAuthority() || GetNetMode() == NM_Standalone)) {
		if (Character->GetMesh())
		{
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			CurrentWeapon->AttachToComponent(Character->GetMesh(), Rules, "skt_weapon");
		}else
		{
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			CurrentWeapon->AttachToComponent(Character->GetCapsuleComponent(), Rules);
		}
	}
}

void UWeaponManager::TakeWeapon(AWeapon* Weapon)
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());
	if (GetOwner()->HasAuthority() && Weapon && Character) {
		Weapon->SetOwner(Character);
		CurrentWeapon = Weapon;
		if (GetNetMode() == NM_Standalone) {
			AttachWeapon();
		}
	}
}
