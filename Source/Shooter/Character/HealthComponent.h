// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shooter/UI/Widget/HealthWidget.h"


#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UHealthComponent : public UActorComponent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
    TSubclassOf<UHealthWidget> HealthWidgetClass;

    UPROPERTY()
    class UHealthWidget* HealthWidget;

    UFUNCTION()
    void OnRep_Health() const;

    UFUNCTION()
    void OnRep_IsDead() const;

protected:
    UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_IsDead, Category = Health,
        meta = (AllowPrivateAccess = "true"))
    bool bIsDead = false;

    UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_Health, Category = Health,
        meta = (AllowPrivateAccess = "true"))
    float Health;

    UPROPERTY(EditAnywhere, Category = Health)
    float MaxHealth;

    TFunction<void()> LambdaDie;

    // Called when the game starts
    virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
                         class AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void UpdateHealthWidget() const;

    UFUNCTION()
    void Die() const;

public:
    // Sets default values for this component's properties
    UHealthComponent();

    UFUNCTION()
    void CreateWidgets();

    UFUNCTION()
    void RemoveWidgets() const;

    FORCEINLINE void SetFunctionDie(TFunction<void()> Function);

    FORCEINLINE bool IsDead() const { return bIsDead; }
};
