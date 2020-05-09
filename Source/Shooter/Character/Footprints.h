// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/ArrowComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/DecalActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Footprints.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTER_API UFootprints: public UActorComponent
{
	GENERATED_BODY()
private:
	/** FX for footprints on the default material */
	UPROPERTY(EditDefaultsOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* DefaultFX;

	/** decal for footprints on the default material */
	UPROPERTY(EditDefaultsOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* DefaultFootprint;

	/** FX for footprints on sand */
	UPROPERTY(EditDefaultsOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
		UParticleSystem* SandFX;

	/** decal for footprints on sand */
	UPROPERTY(EditDefaultsOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
		UMaterialInterface* SandFootprint;

	
public:	
	// Sets default values for this component's properties
	UFootprints();

	UFUNCTION(BlueprintCallable, Category = "Character")
		void FootDown(const UArrowComponent* FootArrow);

	UFUNCTION(BlueprintCallable, Category = "Character")
	UParticleSystem* GetFootprintFX(UPhysicalMaterial* PhysMaterial);

	UFUNCTION(BlueprintCallable, Category = "Character")
	UMaterialInterface* GetFootprintDecal(UPhysicalMaterial* PhysMaterial);


protected:
	void Trace(FHitResult& OutHit, FVector& Location);
};
