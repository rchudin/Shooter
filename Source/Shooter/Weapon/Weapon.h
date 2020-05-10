// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimationAsset.h"
#include "Weapon.generated.h"

UENUM()
enum WeaponType
{
	Main		UMETA(DisplayName = "Main"),
	Secondary	UMETA(DisplayName = "Secondary"),
};

UCLASS()
class SHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
private:
	/** The main skeletal mesh*/
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Animation) 
		class UAnimationAsset* FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = Animation) 
		class UAnimationAsset* ReloadAnimation;

	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// [Server] Use
	/** Function that handles firing */
	UFUNCTION(Reliable, Server, WithValidation)
		void Use();

	bool CanBeUsed();

protected:
	virtual void UseWeapon();

public:	
	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }

};