// Copyright © 2020 ruslanchudin.com


#include "WeaponManager.h"

#include "Shooter/Character/ShooterPlayerController.h"


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
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: %s - CANT FIND OWNER"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
		return nullptr;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Owner;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector Location =  Owner->GetActorLocation();
	const FRotator Rotation = Owner->GetActorRotation();

	auto World = GetWorld();
	AWeapon* Weapon = nullptr;
	if (World){
		Weapon = World->SpawnActor<AWeapon>(WeaponClass, Location, Rotation, SpawnParams);
	}else{
		UE_LOG(LogTemp, Error, TEXT("%s: %s - CANT FIND WORLD"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
	}
	return  Weapon;
}

void UWeaponManager::OnRep_CurrentWeapon() const
{
	if (CurrentWeapon)
	{
		if (AttachWeapon)
		{
			AttachWeapon(CurrentWeapon);
		}
		else
		{
			const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			CurrentWeapon->AttachToComponent(GetOwner()->GetRootComponent(), Rules);
		}
		CurrentWeapon->UpdateHud();
	}
}


void UWeaponManager::TakeWeapon(AWeapon* Weapon)
{
	UE_LOG(LogTemp, Log, TEXT("%s: %s"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
	
	if (GetOwner()->HasAuthority() && Weapon)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->ResetWeapon();
		}
		CurrentWeapon = Weapon;
		APawn* Pawn =  Cast<APawn>(GetOwner());
		CurrentWeapon->SetInstigator(Pawn);
		if (Pawn)
		{
			AController* PlayerController = Pawn->GetController();
			PlayerController ? CurrentWeapon->SetOwner(Pawn->GetController()) : CurrentWeapon->SetOwner(GetOwner());
		}
		
		if (GetNetMode() == NM_Standalone) {
			OnRep_CurrentWeapon();
		}
	}
}
