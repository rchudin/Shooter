// Copyright © 2020 ruslanchudin.com

#include "Weapon.h"
#include "DrawDebugHelpers.h"

AWeapon::AWeapon()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

    Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(RootComponent);
    Mesh->SetWorldLocationAndRotation(FVector(), FRotator());
    Mesh->SetSimulatePhysics(true);
    Mesh->SetCollisionProfileName(TEXT("Weapon"));
}

float AWeapon::PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate, const FName StartSectionName) const
{
    UAnimInstance* AnimInstance = (Mesh) ? Mesh->GetAnimInstance() : nullptr;
    if (AnimMontage && AnimInstance)
    {
        float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

        if (Duration > 0.f)
        {
            // Start at a given Section.
            if (StartSectionName != NAME_None)
            {
                AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
            }

            return Duration;
        }
    }
    return 0.f;
}

void AWeapon::OnRep_Instigator()
{
    Super::OnRep_Instigator();
    if (!GetInstigator())
    {
        Detach();
    }
}


void AWeapon::Detach()
{
    if (HasAuthority())
    {
        SetInstigator(nullptr);
        SetOwner(nullptr);
    }
    IsPressedUse = false;
    GetViewPointLambda = nullptr;
    RemoveUpdatingWidget();
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    SetActorLocation(GetActorForwardVector() * 100 + GetActorLocation());
    Mesh->SetSimulatePhysics(true);
}

bool AWeapon::CanUseWeapon() const
{
    return HasAuthority() && GetViewPointLambda;
}

FHitResult AWeapon::Trace(const FVector& Start, const FVector& End) const
{
    FHitResult OutHit = FHitResult(ForceInit);

    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);
    ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

    FCollisionQueryParams TraceParams(FName(TEXT("Fire trace")));
    TraceParams.bReturnPhysicalMaterial = true;
    TraceParams.AddIgnoredActor(this);
    TraceParams.AddIgnoredActor(GetInstigator());

    UWorld* World = GetWorld();
    if (World)
    {
        /*World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, TraceParams);*/
        World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, TraceParams);
    }
    return OutHit;
}

void AWeapon::DrawDebugTraceLine(const FHitResult& OutHit, const FVector& Start, FVector End) const
{
    if (!OutHit.Location.IsZero())
    {
        End = OutHit.Location;
    }
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
    DrawDebugSphere(GetWorld(), End, 10.f, 8, FColor::Red, false, 3.0f);
}

void AWeapon::Server_Use_Implementation()
{
    IsPressedUse = true;
    Use();
}

void AWeapon::Server_StopUse_Implementation()
{
    IsPressedUse = false;
    StopUse();
}

AWeapon* AWeapon::CreateWeapon(UWorld* World, const TSubclassOf<AWeapon>& WeaponClass, const FVector& Location)
{
    if (World)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        return World->SpawnActor<AWeapon>(WeaponClass, Location, FRotator(), SpawnParams);
    }
    return nullptr;
}
