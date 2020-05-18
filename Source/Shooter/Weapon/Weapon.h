// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
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
	
	/** The main skeletal mesh*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh",meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;
	
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
	FTimerHandle FireTimerHandle;

	//use toggle switch
	bool FireTimerExpired;

	// Called when the game starts or when spawned
    virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Stats)
        int MaxTotalAmmo;

	UPROPERTY(VisibleAnywhere, Transient, Replicated, Category = Stats)
		int TotalAmmo;

	UPROPERTY(EditAnywhere, Category = Stats)
		uint16 MaxCurrentAmmo;
	
	UPROPERTY(VisibleAnywhere, Transient, Replicated, Category = Stats)
        uint16 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Stats)
		float UseRate;

	UPROPERTY(EditAnywhere, Category = Stats)
        float UseRange;
	
	virtual void Use() { check(0 && "You must override this"); }

	virtual void UseEffects() { check(0 && "You must override this"); }

	void Trace(FHitResult& OutHit, FVector& Start, FVector& End) const;

	bool GetTrajectory(FVector& Start, FVector& End) const;

	void DrawDebugFireLine(FHitResult& OutHit, FVector& Start, FVector& End) const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Reliable, NetMulticast)
		void MulticastUseEffects();
	
public:	
	/** Returns Mesh subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }
};