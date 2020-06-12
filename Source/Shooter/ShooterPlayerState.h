// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Weapon/Weapon.h"
#include "ShooterPlayerState.generated.h"


UCLASS()
class SHOOTER_API AShooterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	FWeaponInstance* DefaultMainWeapon;

public:
	void SetDefaultMainWeapon(FWeaponInstance* NewDefaultMainWeapon) {DefaultMainWeapon = NewDefaultMainWeapon; }
	
	/** Returns Default Main Weapon sub object **/
    FORCEINLINE FWeaponInstance* GetDefaultMainWeapon() const { return DefaultMainWeapon; }
	
};
