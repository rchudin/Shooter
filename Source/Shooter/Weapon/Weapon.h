// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"
#include "Ammo.h"
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
	
	/** The main skeletal mesh*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh",meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<EWeaponType> Type;

	UFUNCTION()
		void OnRep_CurrentAmmo() const  { CurrentAmmo.OnUpdate(); }
	
	UFUNCTION()
		void OnRep_TotalAmmo() const { TotalAmmo.OnUpdate(); }

public:
	static  AWeapon* CreateWeapon(UWorld* World, const TSubclassOf<AWeapon>& WeaponClass, const FVector& Location);
	
	// Sets default values for this actor's properties
	AWeapon();

	// [Server] Use
	/** Function that handles firing */
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerUse();

	// [Server] StopUse
	/** Function that handles stop firing */
	UFUNCTION(Reliable, Server, WithValidation)
        void ServerStopUse();

	bool CanBeUsed() const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FTimerHandle FireTimerHandle;
	
	//use toggle switch
	bool FireTimerExpired;

	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_CurrentAmmo, Category = "Stats", meta = (AllowPrivateAccess = "true"))
        FAmmo CurrentAmmo;
	
	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_TotalAmmo, Category = "Stats",  meta = (AllowPrivateAccess = "true"))
        FAmmo TotalAmmo;

	UPROPERTY(EditAnywhere, Category = Stats)
        int MaxCurrentAmmo;
	
	UPROPERTY(EditAnywhere, Category = Stats)
        int MaxTotalAmmo;

	UPROPERTY(EditAnywhere, Category = Stats)
		float UseRate;

	UPROPERTY(EditAnywhere, Category = Stats)
        float UseRange;

	void SetMaxAmmo();

	virtual void OnRep_Instigator() override;
	
	virtual void Use() { check(0 && "You must override this"); }

	virtual void UseEffects() { check(0 && "You must override this"); }

	void Trace(FHitResult& OutHit, FVector& Start, FVector& End) const;

	bool GetTrajectory(FVector& Start, FVector& End) const;

	void DrawDebugFireLine(FHitResult& OutHit, FVector& Start, FVector& End) const;

	UFUNCTION(Reliable, NetMulticast)
		void MulticastUseEffects();
	
public:
	void RemoveUpdatingWidget();
	
	void SetOnCurrentAmmoUpdateFunction(TFunction<void(const int& Value)> F) {CurrentAmmo.UpdateWidget = F; CurrentAmmo.OnUpdate(); }
	
	void SetOnTotalAmmoUpdateFunction(TFunction<void(const int& Value)> F) {TotalAmmo.UpdateWidget = F; TotalAmmo.OnUpdate(); }

	void Throw();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon stats")
		 int GetCurrentAmmo() const { return CurrentAmmo; }
		 
	UFUNCTION(BlueprintCallable, Category = "Weapon stats")
		 int GetTotalAmmo() const { return TotalAmmo; }
	
	/** Returns Mesh sub object **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }
	
	/** Returns Type sub object **/
	FORCEINLINE TEnumAsByte<EWeaponType> GetType() const { return Type; }
};

USTRUCT(BlueprintType)
struct FWeaponInstance : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AWeapon> Weapon;
};