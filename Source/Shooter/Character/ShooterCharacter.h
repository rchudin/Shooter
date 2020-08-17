// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Footprints.h"
#include "Shooter/Weapon/WeaponManager.h"
#include "Shooter/Character/HealthComponent.h"
#include "ShooterCharacter.generated.h"

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** First person camera*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ThirdPersonCamera;

	/** Footprint */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		class UFootprints* Footprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* RightFootArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* LeftFootArrow;

	/** Health component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
        class UHealthComponent *HealthComponent;
	
	/** Weapon manager*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
		class UWeaponManager* WeaponManager;


public:
	AShooterCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	void GetPlayerViewPoint(FVector& Out_Location, FVector& Out_Forward) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController * NewController) override;

	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Reload();
	
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	/** Camera change from first person or third person */
	void ToggleCamera() {ThirdPersonCamera->IsActive() ? ActivateFirstPersonCamera() : ActivateThirdPersonCamera();}

	void ActivateFirstPersonCamera() const;

	void ActivateThirdPersonCamera() const;

	/** Function that handles firing */
	void Fire() { if (WeaponManager) WeaponManager->UseWeapon(); }

	void StopFire() { if (WeaponManager) WeaponManager->StopUseWeapon(); }
	
	/* Start Crouch */
	void StartCrouch();
	
	/* Stop Crouch*/
	void StopCrouch();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
        FRotator GetAimRotation(int BoneCount) const;

	/** Returns IsArmed **/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool GetIsArmed() const {return WeaponManager && WeaponManager->GetCurrentWeapon(); }

public:
	/** Returns CameraBoom sub object **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera sub object **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FirstPersonCamera->IsActive() ? FirstPersonCamera : ThirdPersonCamera; }
	/** Returns Footprint sub object **/
	FORCEINLINE class UFootprints* GetFootprint() const { return Footprint; }
	/** Returns RightFootArrow sub object **/
	FORCEINLINE class UArrowComponent* GetRightFootArrow() const { return RightFootArrow; }
	/** Returns LeftFootArrow sub object **/
	FORCEINLINE class UArrowComponent* GetLeftFootArrow() const { return LeftFootArrow; }
	/** Returns WeaponManager sub object **/
	FORCEINLINE class UWeaponManager* GetWeaponManager() const { return WeaponManager; }
};
