// Copyright © 2020 ruslanchudin.com
#define SURFACE_TYPE_Sand    SurfaceType1

#include "Footprints.h"



// Sets default values for this component's properties
UFootprints::UFootprints()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFootprints::FootDown(const UArrowComponent* FootArrow)
{
	if (FootArrow) {
		FHitResult HitResult;
		FVector FootWorldPosition = FootArrow->GetComponentTransform().GetLocation();

		FVector Forward = FootArrow->GetForwardVector();
		Trace(HitResult, FootWorldPosition);

		// Debug vis
		/*DrawDebugLine(GetWorld(), HitResult.Location, HitResult.Location + HitResult.Normal * 100.0f, FColor::Blue, false, 10.0f, 0, 5.0f);*/

		UPhysicalMaterial* PhysMat = HitResult.PhysMaterial.Get();

		// Retrieve the particle system and decal object to spawn for our current ground type
		UParticleSystem* ParticleFX = GetFootprintFX(PhysMat);
		UMaterialInterface* DecalMaterial = GetFootprintDecal(PhysMat);

		// Create a rotator using the landscape normal and our foot forward vectors
		// Note that we use the function ZX to enforce the normal direction (Z)
		FQuat floorRot = FRotationMatrix::MakeFromZX(HitResult.Normal, Forward).ToQuat();
		FQuat offsetRot(FRotator(0.0f, -90.0f, 0.0f));
		FRotator Rotation = (floorRot * offsetRot).Rotator();

		// Spawn decal and particle emitter
		if (DecalMaterial) {
			ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(ADecalActor::StaticClass(), HitResult.Location, Rotation);
			Decal->SetDecalMaterial(DecalMaterial);
			Decal->SetActorScale3D(FVector(0.1f, 0.1f, 0.1f));
			Decal->InitialLifeSpan = 3.0f;
		}

		if (ParticleFX) {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX, HitResult.Location);
		}
	}
}

UParticleSystem* UFootprints::GetFootprintFX(UPhysicalMaterial* PhysMaterial)
{
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMaterial);

	UParticleSystem* Result = nullptr;

	switch (SurfaceType)
	{
	case SURFACE_TYPE_Sand:
		Result = SandFX;
		break;
	default:
		Result = DefaultFX;
		break;
	}

	return Result;
}

UMaterialInterface* UFootprints::GetFootprintDecal(UPhysicalMaterial* PhysMaterial)
{
	EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysMaterial); 

	UMaterialInterface* Result = nullptr;

	switch (SurfaceType)
	{
		case SURFACE_TYPE_Sand:
			Result =  SandFootprint;
			break;
		default: 
			Result = DefaultFootprint;
			break;
	}

	return Result;
}

void UFootprints::Trace(FHitResult& OutHit, FVector& Location)
{
	FVector Start = Location;
	FVector End = Location;

	Start.Z += 20.0f;
	End.Z -= 20.0f;



	OutHit = FHitResult(ForceInit);

	FCollisionQueryParams TraceParams(FName(TEXT("Footprint trace")));
	TraceParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, TraceParams);
}