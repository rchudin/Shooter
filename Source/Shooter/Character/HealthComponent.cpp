// Copyright © 2020 ruslanchudin.com


#include "HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Shooter/FunctionLibrary.h"


UHealthComponent::UHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicated(true);

    MaxHealth = 100.f;
    Health = MaxHealth;
}


void UHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);
}


void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UHealthComponent, bIsDead);
    DOREPLIFETIME_CONDITION(UHealthComponent, Health, COND_OwnerOnly);
}


void UHealthComponent::OnRep_Health() const
{
    UpdateHealthWidget();
}


void UHealthComponent::OnRep_IsDead() const
{
    if (bIsDead) Die();
}


void UHealthComponent::CreateWidgets()
{
    LOG_INSTANCE(LogTemp, Log, GetOwner()->HasAuthority(), TEXT("%s"), TEXT(__FUNCTION__))
    if (GetNetMode() == NM_Standalone ||
        GetOwnerRole() == ROLE_AutonomousProxy)
    {
        if (HealthWidgetClass && !HealthWidget)
        {
            HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), HealthWidgetClass);
            if (HealthWidget)
            {
                HealthWidget->AddToViewport();
                UpdateHealthWidget();
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


void UHealthComponent::SetFunctionDie(const TFunction<void()> Function)
{
    LambdaDie = Function;
}


void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage > 0 && GetOwner()->HasAuthority())
    {
        Health = FMath::Max(Health - Damage, 0.f);
        LOG_INSTANCE(LogTemp, Log, GetOwner()->HasAuthority(), TEXT("%s D: %f H: %f"),
                     TEXT(__FUNCTION__), Damage, Health);

        bIsDead = Health <= 0;
        if (bIsDead) Die();
        UpdateHealthWidget();
    }
}


void UHealthComponent::UpdateHealthWidget() const
{
    if (HealthWidget) HealthWidget->UpdateHealth(Health, Health / MaxHealth);
}


void UHealthComponent::Die() const
{
    if (LambdaDie) LambdaDie();
}
