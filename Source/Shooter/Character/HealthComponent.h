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

	UFUNCTION()
        void OnRep_Health() const;

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
        TSubclassOf<UHealthWidget> HealthWidgetClass;

	UPROPERTY()
        class UHealthWidget *HealthWidget;
	
public:	
	// Sets default values for this component's properties
	UHealthComponent();

	void CreateWidgets();

	void RemoveWidgets() const;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_Health, Category = Health,  meta = (AllowPrivateAccess = "true"))
        float Health;
	
	UPROPERTY(EditAnywhere, Category = Health)
        float MaxHealth;

	UFUNCTION()
		void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
};
