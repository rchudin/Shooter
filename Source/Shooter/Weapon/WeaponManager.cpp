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

AWeapon* UWeaponManager::CreateWeapon(UClass* WeaponClass)
{
	if (!WeaponClass->IsChildOf(AWeapon::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("%s: %s - WEAPON CLASS NOT WEAPON"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
		return  nullptr;
	}
	
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());
	if (!Character)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: %s - CANT FIND CHARACTER OWNER"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
		return nullptr;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector Location =  Character->GetCapsuleComponent()->GetComponentLocation();
	const FRotator Rotation = Character->GetCapsuleComponent()->GetComponentRotation();

	auto World = GetWorld();
	AWeapon* Weapon = nullptr;
	if (World){
		Weapon = World->SpawnActor<AWeapon>(WeaponClass, Location, Rotation, SpawnParams);
	}else{
		UE_LOG(LogTemp, Error, TEXT("%s: %s - CANT FIND WORLD"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
	}
	return  Weapon;
}

void UWeaponManager::AttachWeapon() const
{
	AShooterCharacter* Character = CastChecked<AShooterCharacter>(GetOwner());
	if (Character && CurrentWeapon && (!GetOwner()->HasAuthority() || GetNetMode() == NM_Standalone)) {
		if (Character->GetMesh())
		{
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			CurrentWeapon->AttachToComponent(Character->GetMesh(), Rules, "skt_weapon");
		}else
		{
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			CurrentWeapon->AttachToComponent(Character->GetRootComponent(), Rules);
		}
	}
}

void UWeaponManager::TakeWeapon(AWeapon* Weapon)
{
	UE_LOG(LogTemp, Log, TEXT("%s: %s"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
	AShooterCharacter* Character = CastChecked<AShooterCharacter>(GetOwner());
	if (GetOwner()->HasAuthority() && Weapon && Character) {
		Weapon->SetOwner(Character);
		Weapon->SetInstigator(Character);
		CurrentWeapon = Weapon;
		if (GetNetMode() == NM_Standalone) {
			AttachWeapon();
		}
	}
}
