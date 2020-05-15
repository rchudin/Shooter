// Copyright © 2020 ruslanchudin.com


#include "FireWeapon.h"
#include "Math/Rotator.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Character/ShooterCharacter.h"



void AFireWeapon::Use()
{
    if (CanBeUsed()) {

        UE_LOG(LogTemp, Log, TEXT("%s: Use Weapon"), HasAuthority()?TEXT("Server"):TEXT("Client"));
        
        UWorld* World = GetWorld();
        if (World) {
            FTimerDelegate TimerCallback;
            TimerCallback.BindLambda([this,World]
            {
                World->GetTimerManager().ClearTimer(FireTimerHandle);
            });
            World->GetTimerManager().SetTimer(FireTimerHandle, TimerCallback, UseRate, false);

            Fire();
        }

    }
}

void AFireWeapon::Fire()
{
    AShooterCharacter* Character = CastChecked<AShooterCharacter>(GetOwner());

    if (Character)
    {
        FHitResult OutHit;
        const FVector ForwardVector = Character->GetBaseAimRotation().RotateVector(FVector(1.f, 0, 0));
        FVector Start = Character->GetFollowCamera()->GetComponentLocation();
        FVector End = ((ForwardVector * UseRange) + Start);
        Trace(OutHit, Start, End);
        MulticastUseEffects();
    }
    
}


void AFireWeapon::UseEffects()
{
    if (FireAnimation && GetMesh()) {
    	GetMesh()->PlayAnimation(FireAnimation, false);
    }

    AShooterCharacter* Character = CastChecked<AShooterCharacter>(GetOwner());
    if (Character)
    {
        FHitResult OutHit;
        const FVector ForwardVector = Character->GetBaseAimRotation().RotateVector(FVector(1.f, 0, 0));
        FVector Start = Character->GetFollowCamera()->GetComponentLocation();;
        FVector End = ((ForwardVector * UseRange) + Start);
        Trace(OutHit, Start, End);
        if (GetMesh())
        {
            const FVector NewStart = GetMesh()->GetSocketLocation("skt_muzzle");
            if (!NewStart.IsZero())
            {
                Start = NewStart;
                if (Projectile)
                {
                    FRotator Rotate =  UKismetMathLibrary::FindLookAtRotation(Start, End);
                }
            }
        }
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
    }
}
