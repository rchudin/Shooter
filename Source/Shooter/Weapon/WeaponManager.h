// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Shooter//Character/ShooterCharacter.h"
#include "Weapon.h"
#include "WeaponManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_CurrentWeapon, Category = "Weapon")
		class AWeapon* CurrentWeapon;

	UPROPERTY(replicated)
		class AWeapon* MainWeapon;

	UPROPERTY(replicated)
		class AWeapon* SecondaryWeapon;

	UFUNCTION()
		void AttachWeapon() const;

	/** Called when the CurrentWeapon variable gets updated */
	UFUNCTION()
		void OnRep_CurrentWeapon() const { AttachWeapon(); }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Sets default values for this component's properties
	UWeaponManager();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void TakeWeapon(AWeapon* Weapon);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		AWeapon* CreateWeapon(UClass* WeaponClass);

	/** Returns CurrentWeapon subobject **/
    FORCEINLINE class AWeapon* GetCurrentWeapon() const { return CurrentWeapon; }
};
