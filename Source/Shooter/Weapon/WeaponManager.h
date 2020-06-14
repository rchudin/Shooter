// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon.h"
#include "Shooter/UI/Widget/AmmoWidget.h"
#include "WeaponManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
            TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
        class UUserWidget* CrosshairWidget;
    
	UPROPERTY(EditAnywhere, Category = "Widget", meta = (AllowPrivateAccess = "true"))
        TSubclassOf<UAmmoWidget> AmmoWidgetClass;
    
	UPROPERTY()
        class UAmmoWidget* AmmoWidget;
	
	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_CurrentWeapon, Category = "Weapon")
		class AWeapon* CurrentWeapon;

	UPROPERTY(replicated)
		class AWeapon* MainWeapon;

	UPROPERTY(replicated)
		class AWeapon* SecondaryWeapon;

	/** Called when the CurrentWeapon variable gets updated */
	UFUNCTION()
		void OnRep_CurrentWeapon();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void SetUpdatingWidgetInWeapon();
	
	void RemoveUpdatingWidgetInWeapon() const { if (CurrentWeapon) CurrentWeapon->RemoveUpdatingWidget(); }

public:	
	// Sets default values for this component's properties
	UWeaponManager();

	void CreateWidgets();

	void RemoveWidgets() const;

	void UseWeapon() const;
	
	void StopUseWeapon() const;

	TFunction<void(AActor*)> AttachWeapon;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void TakeWeapon(AWeapon* Weapon);

	/** Returns CurrentWeapon subobject **/
    FORCEINLINE class AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};
