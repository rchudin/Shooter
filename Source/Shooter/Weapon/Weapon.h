// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/Class.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimationAsset.h"
#include "Net/UnrealNetwork.h"
#include "Weapon.generated.h"

UENUM()
enum EWeaponType
{
	Main		UMETA(DisplayName = "Main"),
	Secondary	UMETA(DisplayName = "Secondary"),
};

UCLASS(Abstract)
class SHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
private:
	FTimerHandle FireTimerHandle;

	// class AShooterCharacter* ParentCharacter;

	/** The main skeletal mesh*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh",meta = (AllowPrivateAccess = "true"))
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

	// [Server] StopUse
	/** Function that handles stop firing */
	UFUNCTION(Reliable, Server, WithValidation)
        void StopUse();

	bool CanBeUsed() const;

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Stats)
        unsigned int MaxAmmo;

	UPROPERTY(VisibleAnywhere, Transient, Replicated, Category = Stats)
        unsigned int CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Stats)
		float UseRate;
	
	virtual void UseWeapon() { check(0 && "You must override this"); }

	void Trace(FHitResult& OutHit, FVector& Start, FVector& End) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Reliable, NetMulticast)
            void UseWeaponEffects();
	
	/** Returns ParentCharacter subobject **/
	// FORCEINLINE class AShooterCharacter* GetParentCharacter() const { return ParentCharacter; }
public:	
	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }
	/** Set ParentCharacter */
	// FORCEINLINE void SetParentCharacter(AShooterCharacter* Character) { ParentCharacter = Character; }

};