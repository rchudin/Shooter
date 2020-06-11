// Copyright © 2020 ruslanchudin.com

#include "Weapon.h"
#include "Shooter/Character/ShooterCharacter.h"


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

	MaxTotalAmmo = 180;
	MaxCurrentAmmo = 30;
	UseRate = 0.5f;
	UseRange = 1000.f;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	ResetAmmo();
}

void AWeapon::AddCurrentAmmo(const int& Count)
{
	if(Count < 0 && CurrentAmmo < Count * -1 )
	{
		UE_LOG(LogTemp, Error, TEXT("%s: %s Invalid value"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
		return;
	}
	CurrentAmmo += Count;

	if (!HasAuthority() || GetNetMode() == NM_Standalone)
	{
		UpdateHudCurrentAmmo();
	}
}

void AWeapon::AddTotalAmmo(const int& Count)
{
	if (Count < 0 && TotalAmmo < Count * -1)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: %s Invalid value"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));
		return;
	}
	TotalAmmo += Count;

	if (!HasAuthority() || GetNetMode() == NM_Standalone)
	{
         UpdateHudTotalAmmo();       
	}
}

void AWeapon::OnRep_CurrentAmmo()
{
	UpdateHudCurrentAmmo();
}

void AWeapon::OnRep_TotalAmmo()
{
	UpdateHudTotalAmmo();
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

bool AWeapon::GetTrajectory(FVector& Start, FVector& End) const
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetInstigator());
	if (Character)
	{
		FVector ForwardVector;
		if (Character->GetController())
		{
			ForwardVector = Character->GetFollowCamera()->GetForwardVector();
		}else
		{
			ForwardVector = Character->GetBaseAimRotation().RotateVector(FVector(1.f,0.f,0.f));
		}
		
		Start = Character->GetFollowCamera()->GetComponentLocation();
		End = ((ForwardVector * UseRange) + Start);

		return true;
	}
	return false;
}

void AWeapon::DrawDebugFireLine(FHitResult& OutHit, FVector& Start, FVector& End) const 
{
	
	UE_LOG(LogTemp, Log, TEXT("%s: Fire Start(x:%f, y:%f , z:%f)"), HasAuthority()?TEXT("Server"):TEXT("Client"), Start.X, Start.Y, Start.Z);
	UE_LOG(LogTemp, Log, TEXT("%s: Fire End(x:%f, y:%f , z:%f)"), HasAuthority()?TEXT("Server"):TEXT("Client"), End.X, End.Y, End.Z);
        
	if (!OutHit.Location.IsZero())
	{
		End = OutHit.Location;
	}
        
	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 3.0f);
	DrawDebugSphere(GetWorld(), End, 10.f, 8, FColor::Red, false, 3.0f);
}

void AWeapon::UpdateHudCurrentAmmo()
{
	if (InGameHud)
	{
		InGameHud->UpdateCurrentAmmo(CurrentAmmo);
	}else
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
		if (PlayerController)
		{
			AInGameHud* NewHud =  Cast<AInGameHud>(PlayerController->GetHUD());
			if (NewHud)
			{
				InGameHud = NewHud;
				InGameHud->UpdateCurrentAmmo(CurrentAmmo);
			}
		}
	}
}

void AWeapon::UpdateHudTotalAmmo()
{
	if (InGameHud)
	{
		InGameHud->UpdateTotalAmmo(TotalAmmo);
	}else
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
		if (PlayerController)
		{
			AInGameHud* NewHud =  Cast<AInGameHud>(PlayerController->GetHUD());
			if (NewHud)
			{
				InGameHud = NewHud;
				InGameHud->UpdateTotalAmmo(TotalAmmo);
			}
		}
	}
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AWeapon, TotalAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeapon, CurrentAmmo, COND_OwnerOnly);
}

void AWeapon::UpdateHud()
{
	UpdateHudCurrentAmmo();
	UpdateHudTotalAmmo();
}

void AWeapon::ResetWeapon()
{
	SetInstigator(nullptr);
	SetOwner(nullptr);
	InGameHud = nullptr;
}

void AWeapon::ResetAmmo()
{
	CurrentAmmo = MaxCurrentAmmo;
	TotalAmmo = MaxTotalAmmo;
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

	return true;
}

// [Server] Use
bool AWeapon::ServerUse_Validate() { return true; }
void AWeapon::ServerUse_Implementation()
{
	FireTimerExpired = false;

	if (!FireTimerHandle.IsValid() || !GetWorldTimerManager().IsTimerActive(FireTimerHandle))
	{
		Use();
	}
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