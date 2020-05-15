// Copyright © 2020 ruslanchudin.com


#include "FireWeapon.h"
#include "../Character/ShooterCharacter.h"



void AFireWeapon::Use()
{
    if (CanBeUsed()) {

        UE_LOG(LogTemp, Log, TEXT("%s: Use Weapon"), HasAuthority()?TEXT("Server"):TEXT("Client"));
        // UE_LOG(LogTemp, Log, TEXT("Text, %d %f %s"), int, float, *string );
        
        UE_LOG(LogTemp, Log, TEXT("Use Weapon"));
        
        UWorld* World = GetWorld();
        if (World) {
            FTimerDelegate TimerCallback;
            TimerCallback.BindLambda([this,World]
            {
                World->GetTimerManager().ClearTimer(FireTimerHandle);
            });
            World->GetTimerManager().SetTimer(FireTimerHandle, TimerCallback, UseRate, false);

            MulticastUseEffects();
        }

    }
}

void AFireWeapon::UseEffects()
{
    if (FireAnimation && GetMesh()) {
    	GetMesh()->PlayAnimation(FireAnimation, false);
    }

    AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwner());

    FHitResult OutHit;
    FVector Start = Character->GetFollowCamera()->GetComponentLocation();
    const FVector ForwardVector = Character->GetFollowCamera()->GetForwardVector();
    FVector End = ((ForwardVector * 1000.f) + Start);
    
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);

    Trace(OutHit, Start, End);
}
