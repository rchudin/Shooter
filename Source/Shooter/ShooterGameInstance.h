// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "ShooterGameInstance.generated.h"


UCLASS()
class SHOOTER_API UShooterGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		class UDataTable* WeaponInstanceData;
public:
	UShooterGameInstance();

	/** Returns Weapon Instance Data sub object **/
	FORCEINLINE UDataTable* GetWeaponInstanceData() const { return WeaponInstanceData; }
};
