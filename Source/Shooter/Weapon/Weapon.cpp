// Copyright © 2020 ruslanchudin.com

#include "Weapon.h"
#include "Shooter/Character/ShooterCharacter.h"



AWeapon* AWeapon::CreateWeapon(UWorld* World, const TSubclassOf<AWeapon>& WeaponClass, const FVector& Location)
{
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AWeapon* NewWeapon = World->SpawnActor<AWeapon>(WeaponClass, Location, FRotator(), SpawnParams);
		if (NewWeapon) NewWeapon->RestoreToDefaultStats();
		return  NewWeapon;
	}
	return nullptr;
}

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	if (Mesh)
	{
		Mesh->SetupAttachment(RootComponent);
		Mesh->SetWorldLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	}
}


void AWeapon::OnRep_Instigator()
{
	Super::OnRep_Owner();
	
	RemoveUpdatingWidget();
}


void AWeapon::Throw()
{
	RemoveUpdatingWidget();
	SetOwner(nullptr);
	SetInstigator(nullptr);
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, Scatter);
}


void AWeapon::Trace(FHitResult& OutHit, FVector& Start, FVector& End) const
{
	OutHit = FHitResult(ForceInit);

	FCollisionQueryParams TraceParams(FName(TEXT("Fire trace")));
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(GetInstigator());

	UWorld* World = GetWorld();
	if (World)
	{
		World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, TraceParams);
	}
}


FVector AWeapon::GetForwardVector() const
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetInstigator());
	FVector ForwardVector;
	if (Character)
	{
		if (Character->GetController())
		{
			UCameraComponent* Camera = Character ? Character->GetFollowCamera(): nullptr;
			ForwardVector = Camera ? Camera->GetForwardVector() : FVector();
		}
		else
		{
			ForwardVector = Character->GetBaseAimRotation().RotateVector(FVector(1.f,0.f,0.f));
		}
	}
	
	return ForwardVector;
}


FVector AWeapon::GetStartPoint() const
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetInstigator());
	UCameraComponent* Camera = Character ? Character->GetFollowCamera(): nullptr;
	return Camera ? Camera->GetComponentLocation() : FVector();
}


void AWeapon::CalculateTrajectory(FVector& Start, FVector& End) const
{
	const FVector ForwardVector = GetForwardVector();
	Start = GetStartPoint();
	End = ((ForwardVector * UseRange) + Start);

	UE_LOG(LogTemp, Log, TEXT("%s: Fire Start(x:%f, y:%f , z:%f)"), HasAuthority()?TEXT("Server"):TEXT("Client"),  Start.X, Start.Y, Start.Z)
    UE_LOG(LogTemp, Log, TEXT("%s: Fire End(x:%f, y:%f , z:%f)"), HasAuthority()?TEXT("Server"):TEXT("Client"),   End.X, End.Y, End.Z)
}


void AWeapon::DrawDebugFireLine(FHitResult& OutHit, FVector& Start, FVector& End) const 
{
	if (!OutHit.Location.IsZero())
	{
		End = OutHit.Location;
	}
        
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
	DrawDebugSphere(GetWorld(), End, 10.f, 8, FColor::Red, false, 3.0f);
}


void AWeapon::OnRep_Scatter()
{
	PlayUseEffects();
}


// [Server] Use
void AWeapon::Server_Use_Implementation()
{
	Pressed = true;

	if (!UsagePeriodTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(UsagePeriodTimerHandle))
	{
		Use();
	}
}


// [Server] StopUse
void AWeapon::Server_StopUse_Implementation()
{
	Pressed = false;
}