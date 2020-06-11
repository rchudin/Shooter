// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Shooter/UI/InGameHud.h"

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Display", meta = (AllowPrivateAccess = "true"))
        AInGameHud* InGameHud;

	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_CurrentAmmo, Category = "Stats", meta = (AllowPrivateAccess = "true"))
        int CurrentAmmo;
	
	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_TotalAmmo, Category = "Stats",  meta = (AllowPrivateAccess = "true"))
		int TotalAmmo;

	UFUNCTION()
		void OnRep_CurrentAmmo();
	
	UFUNCTION()
		void OnRep_TotalAmmo();

public:	
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
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FTimerHandle FireTimerHandle;
	
	//use toggle switch
	bool FireTimerExpired;
	
	UPROPERTY(EditAnywhere, Category = Stats)
        int MaxTotalAmmo;

	UPROPERTY(EditAnywhere, Category = Stats)
		int MaxCurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Stats)
		float UseRate;

	UPROPERTY(EditAnywhere, Category = Stats)
        float UseRange;

	void AddCurrentAmmo(const int& Count);
	
	void AddTotalAmmo(const int& Count);
	
	virtual void Use() { check(0 && "You must override this"); }

	virtual void UseEffects() { check(0 && "You must override this"); }

	void Trace(FHitResult& OutHit, FVector& Start, FVector& End) const;

	bool GetTrajectory(FVector& Start, FVector& End) const;

	void DrawDebugFireLine(FHitResult& OutHit, FVector& Start, FVector& End) const;

	void UpdateHudCurrentAmmo();

	void UpdateHudTotalAmmo();

	UFUNCTION(Reliable, NetMulticast)
		void MulticastUseEffects();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void UpdateHud();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		virtual void ResetWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon stats")
		void ResetAmmo();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon stats")
		 int GetCurrentAmmo() const { return CurrentAmmo; }
		 
	UFUNCTION(BlueprintCallable, Category = "Weapon stats")
		 int GetTotalAmmo() const { return TotalAmmo; }
	
	/** Returns Mesh sub object **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }
};