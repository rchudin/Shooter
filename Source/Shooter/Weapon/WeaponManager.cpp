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


void UWeaponManager::OnRep_CurrentWeapon()
{
	AttachCurrentWeaponToHand();
}


void UWeaponManager::CreateWidgets()
{
	if (GetNetMode() == NM_Standalone ||
		GetOwnerRole() == ROLE_AutonomousProxy)
	{
		if(CrosshairWidgetClass && !CrosshairWidget)
		{
			CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
			if (CrosshairWidget) CrosshairWidget->AddToViewport();
		}

		if (AmmoWidgetClass && !AmmoWidget)
		{
			AmmoWidget = CreateWidget<UAmmoWidget>(GetWorld(), AmmoWidgetClass);
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
	auto FWeapon = Cast<AFireWeapon>(CurrentWeapon);
	if (FWeapon)
	{
		UAmmoWidget** RefWidget = &AmmoWidget; 
		FWeapon->SetOnCurrentAmmoUpdateFunction([RefWidget](const int& Count)
        {
            if (RefWidget)
            {
                UAmmoWidget* Widget = *RefWidget;
                if (Widget) Widget->UpdateCurrentAmmo(Count);
            }
        });
		
		FWeapon->SetOnTotalAmmoUpdateFunction([RefWidget](const int& Count)
        {
            if (RefWidget)
            {
                UAmmoWidget* Widget = *RefWidget;
                if (Widget) Widget->UpdateTotalAmmo(Count);
            }
        });
	}
}

void UWeaponManager::SetGetViewPointLambdaInWeapon() const
{
	auto FWeapon = Cast<AFireWeapon>(CurrentWeapon);
	if (FWeapon)
	{
		FWeapon->SetGetViewPointLambda(GetViewPointLambda);
	}
}


void UWeaponManager::RemoveUpdatingWidgetInWeapon() const
{
	if (CurrentWeapon) CurrentWeapon->RemoveUpdatingWidget();
}


void UWeaponManager::UseWeapon() const
{
	if (CurrentWeapon) CurrentWeapon->Server_Use();
}


void UWeaponManager::StopUseWeapon() const
{
	if (CurrentWeapon) CurrentWeapon->Server_StopUse();
}


void UWeaponManager::SetInstigatorAndOwnerToWeapon() const
{
	if (CurrentWeapon)
	{
		APawn* Pawn =  Cast<APawn>(GetOwner());
		CurrentWeapon->SetInstigator(Pawn);
		if (Pawn)
		{
			AController* PlayerController = Pawn->GetController();
			PlayerController ? CurrentWeapon->SetOwner(PlayerController) : CurrentWeapon->SetOwner(Pawn);
		}
	}
}

void UWeaponManager::AttachCurrentWeaponToHand()
{
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		if (AttachWeaponToHandFunction)
		{
			AttachWeaponToHandFunction(CurrentWeapon, Rules);
		}
		else
		{
			const auto ActorOwner = GetOwner();
			if (ActorOwner)
				CurrentWeapon->AttachToComponent(ActorOwner->GetRootComponent(), Rules);
		}
		
		SetUpdatingWidgetInWeapon();

		SetGetViewPointLambdaInWeapon();
	}
}


void UWeaponManager::TakeWeapon(AWeapon* Weapon)
{
	UE_LOG(LogTemp, Log, TEXT("%s: %s"), GetOwner()->HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
	
	if (GetOwner()->HasAuthority() && Weapon)
	{
		if (CurrentWeapon) CurrentWeapon->Detach();
		
		CurrentWeapon = Weapon;

		SetInstigatorAndOwnerToWeapon();

		AttachCurrentWeaponToHand();
	}
}
