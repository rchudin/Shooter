// Copyright © 2020 ruslanchudin.com

#include "Weapon.h"



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

	MaxAmmo = 30;
	UseRate = 0.5f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentAmmo = MaxAmmo;
}

void AWeapon::Trace(FHitResult& OutHit, FVector& Start, FVector& End) const
{
	OutHit = FHitResult(ForceInit);

	FCollisionQueryParams TraceParams(FName(TEXT("Fire trace")));
	TraceParams.bReturnPhysicalMaterial = true;

	UWorld* World = GetWorld();
	if (World)
	{
		World->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, TraceParams);
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, CurrentAmmo);
}

void AWeapon::MulticastUseEffects_Implementation()
{
	if (!HasAuthority() || GetNetMode() == NM_Standalone) {
		UseEffects();
	}
}

bool AWeapon::CanBeUsed() const
{
	if (CurrentAmmo <= 0) {
		return false;
	}

	if (FireTimerHandle.IsValid() && GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		return false;
	}
	
	return true;
}

// [Server] Use
bool AWeapon::ServerUse_Validate() { return true; }
void AWeapon::ServerUse_Implementation()
{
	FireTimerExpired = false;
	Use();
}

// [Server] StopUse
bool AWeapon::ServerStopUse_Validate(){return true;}
void AWeapon::ServerStopUse_Implementation()
{
	if (FireTimerHandle.IsValid() && GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		FireTimerExpired = true;
	}
}