// Copyright © 2020 ruslanchudin.com

#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	Mesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	if (Mesh)
	{
		Mesh->SetupAttachment(RootComponent);
		Mesh->SetWorldLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
	}
}

// [Server] Use
bool AWeapon::Use_Validate() { return CanBeUsed(); }
void AWeapon::Use_Implementation() { UseWeapon(); }

bool AWeapon::CanBeUsed()
{
	return true;
}

void AWeapon::UseWeapon()
{

}