// Copyright © 2020 ruslanchudin.com

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "Footprints.h"
#include "Shooter/Weapon/WeaponManager.h"
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

	/**HUD*/
	UPROPERTY(EditAnywhere, Category = "HUD", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<UUserWidget> PlayerDisplayWidget;

	/** Footprint */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		class UFootprints* Footprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* RightFootArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* LeftFootArrow;

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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
	void ToggleCamera();

	void ActivateFirstPersonCamera() const;

	void ActivateThirdPersonCamera() const;

	/** Function that handles firing */
	void Fire();

	void StopFire();

	/*Set HUD player screen*/
	void SetPlayerDisplayWidget() const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
        FRotator GetAimRotation(int BoneCount) const;

	/** Returns IsArmed **/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		bool GetIsArmed() const;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FirstPersonCamera->IsActive() ? FirstPersonCamera : ThirdPersonCamera; }
	/** Returns RightFootArrow subobject **/
	FORCEINLINE class UArrowComponent* GetRightFootArrow() const { return RightFootArrow; }
	/** Returns LeftFootArrow subobject **/
	FORCEINLINE class UArrowComponent* GetLeftFootArrow() const { return LeftFootArrow; }
	/** Returns WeaponManager subobject **/
	FORCEINLINE class UWeaponManager* GetWeaponManager() const { return WeaponManager; }
};

