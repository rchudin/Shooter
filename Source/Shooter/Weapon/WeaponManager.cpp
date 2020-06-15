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


void UWeaponManager::CreateWidgets()
{
	if (GetOwner() && (!GetOwner()->HasAuthority() || GetNetMode() == NM_Standalone))
	{
		if(CrosshairWidgetClass && !CrosshairWidget)
		{
			if (!CrosshairWidget)
			{
				CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
			}
			if (CrosshairWidget) CrosshairWidget->AddToViewport();
		}

		if (AmmoWidgetClass && !AmmoWidget)
		{
			if (!AmmoWidget)
			{
				AmmoWidget = CreateWidget<UAmmoWidget>(GetWorld(), AmmoWidgetClass);
			}
			if (AmmoWidget)
			{
				AmmoWidget->AddToViewport();
				SetUpdatingWidgetInWeapon();
			}
		}
	}
}


void UWeaponManager::RemoveWidgets() const
{
	RemoveUpdatingWidgetInWeapon();
	if (CrosshairWidget)
	{
		if (CrosshairWidget->IsInViewport()) CrosshairWidget->RemoveFromViewport();
		CrosshairWidget->Destruct();
	}

	if(AmmoWidget)
	{
		if (AmmoWidget->IsInViewport()) AmmoWidget->RemoveFromViewport();
		AmmoWidget->Destruct();
	}
}

void UWeaponManager::SetUpdatingWidgetInWeapon()
{
	if (CurrentWeapon)
	{
		UAmmoWidget** RefWidget = &AmmoWidget; 
		CurrentWeapon->SetOnCurrentAmmoUpdateFunction([RefWidget](const int& Count)
        {
            if (RefWidget)
            {
                UAmmoWidget* Widget = *RefWidget;
                if (Widget) Widget->UpdateCurrentAmmo(Count);
            }
        });
		
		CurrentWeapon->SetOnTotalAmmoUpdateFunction([RefWidget](const int& Count)
        {
            if (RefWidget)
            {
                UAmmoWidget* Widget = *RefWidget;
                if (Widget) Widget->UpdateTotalAmmo(Count);
            }
        });
	}
}

void UWeaponManager::RemoveUpdatingWidgetInWeapon() const
{
	if (CurrentWeapon) CurrentWeapon->RemoveUpdatingWidget();
}


void UWeaponManager::UseWeapon() const
{
	if (CurrentWeapon) CurrentWeapon->ServerUse();
}

void UWeaponManager::StopUseWeapon() const
{
	if (CurrentWeapon) CurrentWeapon->ServerStopUse();
}


void UWeaponManager::OnRep_CurrentWeapon() 
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

		SetUpdatingWidgetInWeapon();
	}
}


void UWeaponManager::TakeWeapon(AWeapon* Weapon)
{
	UE_LOG(LogTemp, Log, TEXT("%s: %s"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
	
	if (GetOwner()->HasAuthority() && Weapon)
	{
		if (CurrentWeapon)
		{
			CurrentWeapon->Throw();
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
