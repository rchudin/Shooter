// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include"../Character/ShooterCharacter.h"
#include "Weapon.h"
#include "Containers/Array.h"
#include "Net/UnrealNetwork.h"
#include "WeaponManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTER_API UWeaponManager : public UActorComponent
{
	GENERATED_BODY()
private:
	class AShooterCharacter* ParrentCharacter;

	UPROPERTY(replicated)
	class AWeapon* MainWeapon;

	UPROPERTY(replicated)
	class AWeapon* SecondaryWeapon;

public:	
	// Sets default values for this component's properties
	UWeaponManager();

	void SetupAttachment(class AShooterCharacter* InParent);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

		
};
