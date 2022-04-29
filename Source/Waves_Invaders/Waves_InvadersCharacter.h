// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponSysten.h"
#include "Bullet.h"
#include "Waves_InvadersCharacter.generated.h"



class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AWaves_InvadersCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	///** Gun mesh: 1st person view (seen only by self) */
	//UPROPERTY(BlueprintReadWrite, Category = Mesh)
	//USkeletalMeshComponent* FP_Gun;

	///** Location on gun mesh where projectiles should spawn. */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;

public:
	AWaves_InvadersCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(BlueprintReadWrite, Category=Projectile)
	TSubclassOf<class AWaves_InvadersProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistic) 
	int DeadsEnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool isShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool HasHomingBulletBuff;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
	bool FireDmageOverTimeEnable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Waves)
	bool FirstWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Waves)
	bool SecondtWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Waves)
	bool ThirdstWave;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int rifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int ppAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int bigGunAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
	float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Respawn)
	FVector respawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	int weaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	int KillPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float defaultMs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float defaultHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float defaultJumpPower;

	FTimerHandle fireTimeHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	TArray<AWeaponSysten*> weapon;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	void StartFiring();

	void StopFiring();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	void ManualReload();

	void ReloadWeapon(EWeaponType _weaponType);

	int CalculateAmmo(int _ammoAmount);

	void LookUpAtRate(float Rate);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchToNextWeapon(int _forceSwitchIndex = -1);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TriggerWeaponSwitch();

	UFUNCTION(BlueprintCallable)
	void Die();

	UFUNCTION(BlueprintCallable)
	void Respawn();

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float _damageAmount);

	UFUNCTION(BlueprintCallable)
	void Heal(float _healAmount);

	UFUNCTION(BlueprintCallable)
	void AddAmmo(EAmmoType _ammoType, int _ammoAmount);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void TriggerOutOFAmmoPopUp();

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category = "Weapon")
	void SwitchWeaponMesh(int _index);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void SwitchWeapon(EWeaponType _weaponType);

	UFUNCTION(BlueprintCallable, Category = "UpgradeMenu")
	void UpMoveSpeed(int KillPointsCost, float KoefMsUP);

	UFUNCTION(BlueprintCallable, Category = "UpgradeMenu")
	void UpHp(int KillPointsCost, float KoefHpUP);

	UFUNCTION(BlueprintCallable, Category = "UpgradeMenu")
	void JumpUp(int KillPointsCost, float KoefJumpUP);


	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

