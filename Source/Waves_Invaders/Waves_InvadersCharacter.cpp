// Copyright Epic Games, Inc. All Rights Reserved.

#include "Waves_InvadersCharacter.h"
#include "Waves_InvadersProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AWaves_InvadersCharacter

AWaves_InvadersCharacter::AWaves_InvadersCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	//FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	//FP_Gun->bCastDynamicShadow = false;
	//FP_Gun->CastShadow = false;
	//FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//FP_Gun->SetupAttachment(RootComponent);
	///*FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint_2"));
	//FP_Gun->SetupAttachment(RootComponent);*/
	//FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	//FP_MuzzleLocation->SetupAttachment(FP_Gun);
	//FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	 /*Default offset from the character location for projectiles to spawn*/
	
	
	isShooting = false;
	isReloading = false;
	HasHomingBulletBuff = false;
	FireDmageOverTimeEnable = false;
	
	//Default stats
	defaultHp = 100.0f;
	defaultMs = 600.0f;
	defaultJumpPower = 420.0f;

	health = 100.0f;
	maxHealth = health;
	respawnLocation = FVector(3146.18f, -827.61, 573.8);

	rifleAmmo = 30;
	ppAmmo = 12;
	bigGunAmmo = 3;
	weaponIndex = 0;
	KillPoints = 0;

	DeadsEnemyCount = 0;

	FirstWave = false;
	SecondtWave = false;
	ThirdstWave = false;
}

void AWaves_InvadersCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	/*FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));*/
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AWaves_InvadersCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AWaves_InvadersCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AWaves_InvadersCharacter::StopFiring);

	
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AWaves_InvadersCharacter::ManualReload);
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AWaves_InvadersCharacter::TriggerWeaponSwitch);
	// Enable touchscreen input
	/*EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AWaves_InvadersCharacter::OnResetVR);*/

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AWaves_InvadersCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWaves_InvadersCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWaves_InvadersCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWaves_InvadersCharacter::LookUpAtRate);
}

void AWaves_InvadersCharacter::OnFire()
{
	if(!isReloading)
	{ 
		if(isShooting)
		{
			if (weapon.IsValidIndex(weaponIndex))
			{
				if(weapon[weaponIndex]->cliplAmmo > 0)
				{
					weapon[weaponIndex]->Fire();

					if (weapon[weaponIndex]->weaponMode == EWeaponMode::E_Auto)
					{
						GetWorld()->GetTimerManager().SetTimer(fireTimeHandle, this, &AWaves_InvadersCharacter::OnFire, weapon[weaponIndex]->fireRate, true);
					}

					if (FireSound != nullptr)
					{
						UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
					}

					if (FireAnimation != nullptr)
					{
						if (weapon[weaponIndex]->cliplAmmo > 0 )
						{
							// Get the animation object for the arms mesh
							UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
							if (AnimInstance != nullptr)
							{
								AnimInstance->Montage_Play(FireAnimation, 1.f);
							}
						}
					}
				}
				else  
				{
					
						ReloadWeapon(weapon[weaponIndex]->weaponType);
					
				}
			}
		}
	}
}

void AWaves_InvadersCharacter::StartFiring()
{
	isShooting = true;
	OnFire();
}

void AWaves_InvadersCharacter::StopFiring()
{
	isShooting = false;
	/*fireTimeHandle.Invalidate();*/
}


//void AWaves_InvadersCharacter::OnResetVR()
//{
//	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
//}

void AWaves_InvadersCharacter::UpMoveSpeed(int KillPointsCost, float KoefMsUP)
{
	if (KillPoints >= KillPointsCost)
	{
		if (auto charactermoovment = GetCharacterMovement())
		{
			KillPoints -= KillPointsCost;
			charactermoovment->MaxWalkSpeed = defaultMs * KoefMsUP;
		}
	}
	else
	{
		return;
	}
}

void AWaves_InvadersCharacter::UpHp(int KillPointsCost, float KoefHpUP)
{
	if (KillPoints >= KillPointsCost)
	{
		if (health>=0)
		{
			KillPoints -= KillPointsCost;
			maxHealth = defaultHp * KoefHpUP;
			health = maxHealth;
		}
		else
		{
			return;
		}
	}
}

void AWaves_InvadersCharacter::JumpUp(int KillPointsCost, float KoefJumpUp)
{
	if (KillPoints >= KillPointsCost)
	{
		if (auto charactermoovment = GetCharacterMovement())
		{
			KillPoints -= KillPointsCost;
			charactermoovment->JumpZVelocity = defaultJumpPower * KoefJumpUp;
		}
		else
		{
			return;
		}
	}
}

void AWaves_InvadersCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AWaves_InvadersCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AWaves_InvadersCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AWaves_InvadersCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AWaves_InvadersCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AWaves_InvadersCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWaves_InvadersCharacter::ManualReload()
{
	/*isReloading = true;*/
	if (weapon[weaponIndex]->totalAmmo > 0)
	{
		ReloadWeapon(weapon[weaponIndex]->weaponType);
	}
	
}

void AWaves_InvadersCharacter::ReloadWeapon(EWeaponType _weaponType)
{
	if (weapon[weaponIndex] && weapon[weaponIndex]->cliplAmmo != weapon[weaponIndex]->maxClipAmmo)
	{
		
		
		switch (_weaponType)
		{
		case EWeaponType::E_Rifle:
			if (rifleAmmo != 0)
			{
				isReloading = true;
			}
			rifleAmmo = CalculateAmmo(rifleAmmo);
			break;
			

		case EWeaponType::E_9MM:
			if (ppAmmo != 0)
			{
				isReloading = true;
			}
			ppAmmo = CalculateAmmo(ppAmmo);
			break;
			

		case EWeaponType::E_BigGun:
			if (bigGunAmmo != 0)
			{
				isReloading = true;
			}
			bigGunAmmo = CalculateAmmo(bigGunAmmo);
			break;
			

		default:
			break;
		}
	}
}

int AWaves_InvadersCharacter::CalculateAmmo(int _ammoAmount)
{
	if(_ammoAmount > 0)
	{
		if (weapon[weaponIndex]->cliplAmmo != weapon[weaponIndex]->maxClipAmmo)
		{
			if (_ammoAmount - (weapon[weaponIndex]->maxClipAmmo - weapon[weaponIndex]->cliplAmmo) >= 0)
			{
				_ammoAmount -= (weapon[weaponIndex]->maxClipAmmo - weapon[weaponIndex]->cliplAmmo);
				weapon[weaponIndex]->cliplAmmo = weapon[weaponIndex]->maxClipAmmo;
			}
			else
			{
				weapon[weaponIndex]->cliplAmmo += _ammoAmount;
				_ammoAmount = 0;
			}
		}
	}
	else
	{
		TriggerOutOFAmmoPopUp();
	}

	return _ammoAmount;
}

void AWaves_InvadersCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AWaves_InvadersCharacter::SwitchToNextWeapon(int _forceSwitchIndex)
{
	GetWorld()->GetTimerManager().ClearTimer(fireTimeHandle);
	if (_forceSwitchIndex > -1)
	{
		weaponIndex = _forceSwitchIndex;
		SwitchWeapon(weapon[weaponIndex]->weaponType);
	}
	else
	{ 
		bool success = false;

		for (int i = 0; i < weapon.Num(); ++i)
		{
			if (i > weaponIndex)
			{
				if (weapon[i]->isObtained)
				{
					success = true;
					weaponIndex = i;
					SwitchWeapon(weapon[weaponIndex]->weaponType);
					break;
				}
			}
		}

		if (!success)
		{
			weaponIndex = 0;
			SwitchWeapon(weapon[weaponIndex]->weaponType);
		}
	}
	/*switch(weaponIndex)
	{
	case 0:
		if (weapon.Num() > 1)
		{
			weaponIndex = 1;
			SwitchWeaponMesh(weaponIndex);
		}
		else
		{
			weaponIndex = 0;
			SwitchWeaponMesh(weaponIndex);
		}
		break;

	case 1:
		if (weapon.Num() > 2)
		{
			weaponIndex = 2;
			SwitchWeaponMesh(weaponIndex);
		}
		else
		{
			weaponIndex = 0;
			SwitchWeaponMesh(weaponIndex);
		}
		break;

	case 2:
		if (weapon.Num() > 3)
		{
			weaponIndex = 3;
			SwitchWeaponMesh(weaponIndex);
		}
		else
		{
			weaponIndex = 0;
			SwitchWeaponMesh(weaponIndex);
		}
		break;

	default:
		break;
	}*/
}

void AWaves_InvadersCharacter::TriggerWeaponSwitch()
{
	SwitchToNextWeapon();
}

void AWaves_InvadersCharacter::Die()
{
	/*Respawn();*/
}

void AWaves_InvadersCharacter::Respawn()
{
	health = maxHealth;
	SetActorLocation(respawnLocation);
}

void AWaves_InvadersCharacter::TakeDamage(float _damageAmount)
{
	health -= _damageAmount;

	if (health <= 0.0f) 
	{
		health = 0.0f;
		Die();
	}
}

void AWaves_InvadersCharacter::Heal(float _healAmount)
{
	health += _healAmount;

	if (health > maxHealth)
	{
		health = maxHealth;
	}
}

void AWaves_InvadersCharacter::AddAmmo(EAmmoType _ammoType, int _ammoAmount)
{
	switch (_ammoType)
	{
	case EAmmoType::E_Rifle:
		rifleAmmo += _ammoAmount;
		break;
	
	case EAmmoType::E_9MM:
		ppAmmo += _ammoAmount;
		break;

	case EAmmoType::E_BigGun:
		bigGunAmmo += _ammoAmount;
		break;

	default:
		break;
	}
}



bool AWaves_InvadersCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AWaves_InvadersCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AWaves_InvadersCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AWaves_InvadersCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
