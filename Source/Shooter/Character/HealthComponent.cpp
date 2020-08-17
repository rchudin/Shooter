// Copyright © 2020 ruslanchudin.com


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Shooter/FunctionLibrary.h"



// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicated(true);
	
	MaxHealth = 100.f;
	Health = MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps); 

	DOREPLIFETIME_CONDITION(UHealthComponent, Health, COND_OwnerOnly);
}


void UHealthComponent::OnRep_Health() const
{
	if (HealthWidget) HealthWidget->UpdateHealth(Health,  Health / MaxHealth);
}


void UHealthComponent::CreateWidgets()
{
	LOG_INSTANCE(LogTemp, Log, GetOwner()->HasAuthority(), TEXT("%s"), TEXT(__FUNCTION__))
    if (GetNetMode() == NM_Standalone ||
        GetOwnerRole() == ROLE_AutonomousProxy)
    {
    	if(HealthWidgetClass && !HealthWidget)
    	{
    		HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), HealthWidgetClass);
    		if (HealthWidget)
    		{
    			HealthWidget->AddToViewport();
    			HealthWidget->UpdateHealth(Health, Health / MaxHealth);
    		}
    	}
    }
}


void UHealthComponent::RemoveWidgets() const
{
	if (HealthWidget)
	{
		if (HealthWidget->IsInViewport()) HealthWidget->RemoveFromViewport();
		HealthWidget->Destruct();
	}
}


void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	AActor *Owner = GetOwner();
	if (Damage > 0 && Owner && Owner->HasAuthority())
	{
		Health = FMath::Max(Health - Damage, 0.f);
		LOG_INSTANCE(LogTemp, Log, Owner->HasAuthority(), TEXT("%s D: %f H: %f"), TEXT(__FUNCTION__), Damage, Health);
		if (HealthWidget)
		{
			HealthWidget->UpdateHealth(Health, Health / MaxHealth);
		}
	}
}
