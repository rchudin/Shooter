// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"
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
		void OnRep_Scatter();

public:
	static  AWeapon* CreateWeapon(UWorld* World, const TSubclassOf<AWeapon>& WeaponClass, const FVector& Location);
	
	// Sets default values for this actor's properties
	AWeapon();

	// [Server] Use
	/** Function that handles firing */
	UFUNCTION(Reliable, Server)
		void Server_Use();

	// [Server] StopUse
	/** Function that handles stop firing */
	UFUNCTION(Reliable, Server)
        void Server_StopUse();

	UFUNCTION()
		virtual bool CanBeUsed() const { check(0 && "You must override this"); return false; }

	virtual void RemoveUpdatingWidget() { };

	virtual void Detach();

	/** Returns Mesh sub object **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }

	/** Returns Type sub object **/
	FORCEINLINE TEnumAsByte<EWeaponType> GetType() const { return Type; }

	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void RestoreToDefaultStats() {}

	virtual void PlayUseEffects() {}
	
	//use toggle switch
	UPROPERTY()
        bool Pressed;

	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_Scatter, Category = Stats, meta = (AllowPrivateAccess = "true"))
		bool Scatter;
	
	UPROPERTY()
		FTimerHandle UsagePeriodTimerHandle;
	
	UPROPERTY(EditAnywhere, Category = Stats)
        float UsageTimePeriod;

	UPROPERTY(EditAnywhere, Category = Stats)
        float UseRange;
	
	virtual void OnRep_Instigator() override;

	void Trace(FHitResult& OutHit, FVector& Start, FVector& End) const;

	virtual void GetViewPoint(FVector& Out_Location, FVector& Out_Forward) const;
	
	void CalculateTrajectory(FVector& Start, FVector& End) const;
	
	void DrawDebugFireLine(FHitResult& OutHit, FVector& Start, FVector& End) const;

	virtual void Use() { check(0 && "You must override this"); }
};

USTRUCT(BlueprintType)
struct FWeaponInstance : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AWeapon> Weapon;
};