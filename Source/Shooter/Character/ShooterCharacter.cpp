// Copyright © 2020 ruslanchudin.com


#include "ShooterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Shooter/Core/FunctionLibrary.h"
#include "Shooter/ShooterPlayerController.h"
#include "Shooter/ShooterPlayerState.h"


//////////////////////////////////////////////////////////////////////////
// AShooterCharacter

AShooterCharacter::AShooterCharacter()
{
	// Set size for collision capsule
	const float CapsuleHeight = 96.0f;
	const float CapsuleRadius = 42.0f;
	GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHeight);

	// set location and rotation mash
	GetMesh()->SetWorldLocationAndRotation(FVector(0.0f, 0.0f, CapsuleHeight * -1), FRotator(0.0f, -90.0f, 0.0f));

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; 
	CameraBoom->bUsePawnControlRotation = true;

	// Create a third person camera
	ThirdPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdPersonCamera"));
	ThirdPersonCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ThirdPersonCamera->bUsePawnControlRotation = false;
	ThirdPersonCamera->SetAutoActivate(false);

	// Create a first person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetWorldLocationAndRotation(FVector(0.0f, 7.0f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCamera->SetupAttachment(GetMesh(), "head");
	FirstPersonCamera->bUsePawnControlRotation = true;
	FirstPersonCamera->SetAutoActivate(true);

	// Footprint
	Footprint = CreateDefaultSubobject<UFootprints>(TEXT("Footprint"));

	LeftFootArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftFootArrow"));
	LeftFootArrow->SetupAttachment(GetMesh(), FName("skt_footprint_l"));
	
	RightFootArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RightFootArrow"));
	RightFootArrow->SetupAttachment(GetMesh(), FName("skt_footprint_r"));

	//Health component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->SetFunctionDie([&]()
	{
		Die();
	});
	
	// Weapon Manager
	WeaponManager = CreateDefaultSubobject<UWeaponManager>(TEXT("WeaponManager"));
	WeaponManager->SetFunctionAttachedWeapon([&](AActor* Actor, const FAttachmentTransformRules Rules)
    {
		Actor->AttachToComponent(GetMesh(), Rules, "skt_weapon");
    });
	WeaponManager->SetFunctionGetViewPoint([&](FVector *Out_Location, FVector *Out_Forward)
	{
		GetPlayerViewPoint(Out_Location,Out_Forward);
	});
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AShooterCharacter::PossessedBy(AController* NewController)
{
	LOG_INSTANCE(LogTemp, Log, HasAuthority(), TEXT("%s"), TEXT(__FUNCTION__));
	Super::PossessedBy(NewController);

	CreateWidgets();
	
	/* TEMPORARY */
	if (HasAuthority())
	{
		const AShooterPlayerState* PState = NewController->GetPlayerState<AShooterPlayerState>();
		if (PState)
		{
			const FWeaponInstance* WeaponInstance = PState->GetDefaultMainWeapon();
			if (WeaponInstance)
			{
				AWeapon* NewWeapon = AWeapon::CreateWeapon(GetWorld(), WeaponInstance->Weapon, this->GetActorLocation());
				NewWeapon->RecoverConsumables();
				if (NewWeapon) WeaponManager->TakeWeapon(NewWeapon);
			}
		}
	}
	/* TEMPORARY */
}

void AShooterCharacter::UnPossessed()
{
	RemoveWidgets();
}

void AShooterCharacter::OnRep_Controller()
{
	LOG_INSTANCE(LogTemp, Log, HasAuthority(), TEXT("%s"), TEXT(__FUNCTION__))
	Super::OnRep_Controller();

	GetController() ? CreateWidgets() : RemoveWidgets();
}


void AShooterCharacter::GetPlayerViewPoint(FVector* OutLocation, FVector* OutForward) const
{
	if (OutLocation && OutForward)
	{
		*OutLocation = GetFollowCamera() ? GetFollowCamera()->GetComponentLocation() : GetActorLocation();
		*OutForward = GetController() ? GetController()->GetControlRotation().Vector() : GetBaseAimRotation().Vector();
	}
}


void AShooterCharacter::ActivateFirstPersonCamera() const
{
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(GetController());
	if (PlayerController)
	{
		ThirdPersonCamera->Deactivate();
		FirstPersonCamera->Activate();
		PlayerController->SetFirstPersonViewingAngle();
	}
}

void AShooterCharacter::ActivateThirdPersonCamera() const
{
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(GetController());
	if (PlayerController)
	{
		FirstPersonCamera->Deactivate();
		ThirdPersonCamera->Activate();
		PlayerController->SetThirdPersonViewingAngle();
	}
}

void AShooterCharacter::Fire()
{
	if (!HealthComponent->IsDead() && WeaponManager) WeaponManager->UseWeapon();
}

void AShooterCharacter::StopFire()
{
	if (!HealthComponent->IsDead() && WeaponManager) WeaponManager->StopUseWeapon(); 
}

void AShooterCharacter::StartCrouch()
{
	UE_LOG(LogTemp, Log, TEXT("%s: %s"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));

	Crouch();
}

void AShooterCharacter::StopCrouch()
{
	UE_LOG(LogTemp, Log, TEXT("%s: %s"), HasAuthority()?TEXT("Server"):TEXT("Client"), TEXT(__FUNCTION__));

	UnCrouch();
}

void AShooterCharacter::CreateWidgets() const
{
	HealthComponent->CreateWidgets();
	WeaponManager->CreateWidgets();
}

void AShooterCharacter::RemoveWidgets() const
{
	HealthComponent->RemoveWidgets();
	WeaponManager->RemoveWidgets();
}

void AShooterCharacter::Die() const
{
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesSimulatePhysics(true);

	WeaponManager->RemoveWidgets();
	WeaponManager->DropWeapon();

	const FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	FirstPersonCamera->AttachToComponent(GetCapsuleComponent(), Rules);
}

FRotator AShooterCharacter::GetAimRotation(const int BoneCount) const
{
	float RotationPitch;
	if (GetController())
	{
		RotationPitch= GetControlRotation().Pitch;
	}else
	{
		RotationPitch = GetBaseAimRotation().Pitch;
	}

	return FRotator(0.0f, 0.0f, (RotationPitch > 180.0f ? 360 - RotationPitch : RotationPitch * -1) / BoneCount);
}


//////////////////////////////////////////////////////////////////////////
// Input
void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::StopFire);
	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::Reload);
	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AShooterCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AShooterCharacter::StopCrouch);
	
	PlayerInputComponent->BindAction("ToggleCamera", IE_Pressed, this, &AShooterCharacter::ToggleCamera);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AShooterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AShooterCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AShooterCharacter::OnResetVR);
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}


void AShooterCharacter::Reload()
{
	if (WeaponManager) WeaponManager->Reload();
}


void AShooterCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AShooterCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AShooterCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}


void AShooterCharacter::TurnAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ( Controller && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}