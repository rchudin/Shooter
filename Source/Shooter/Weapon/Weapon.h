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
    Main UMETA(DisplayName = "Main"),
    Second UMETA(DisplayName = "Second"),
};

UCLASS(Abstract)
class SHOOTER_API AWeapon : public AActor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (AllowPrivateAccess = "true"))
    TEnumAsByte<EWeaponType> Type;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh",meta = (AllowPrivateAccess = "true"))
    class USkeletalMeshComponent* Mesh;

protected:
    UPROPERTY(EditAnywhere, Category = Stats)
    float Damage = 0;

    UPROPERTY()
    bool IsPressedUse = false;

    TFunction<void (FVector*, FVector*)> GetViewPointLambda;

    virtual void OnRep_Instigator() override;

    FHitResult Trace(const FVector& Start, const FVector& End) const;

    void DrawDebugTraceLine(const FHitResult& OutHit, const FVector& Start, FVector End) const;

    virtual void Use()
    {
        check(0 && "You must override this");
    }

    virtual void StopUse()
    {
    }

public:
    AWeapon();

    virtual void Detach(); 

    virtual void RemoveUpdatingWidget()
    {
    }

    virtual void RecoverConsumables()
    {
    }

    bool CanUseWeapon() const;

    // [Server] Use
    UFUNCTION(Reliable, Server)
    void Server_Use();

    // [Server] StopUse
    UFUNCTION(Reliable, Server)
    void Server_StopUse();

    FORCEINLINE TEnumAsByte<EWeaponType> GetType() const { return Type; }

    FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }

    FORCEINLINE void SetFunctionGetViewPoint(const TFunction<void (FVector*, FVector*)> Fn) { GetViewPointLambda = Fn; }

    static AWeapon* CreateWeapon(UWorld* World, const TSubclassOf<AWeapon>& WeaponClass, const FVector& Location);
};

USTRUCT(BlueprintType)
struct FWeaponInstance : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AWeapon> Weapon;
};
