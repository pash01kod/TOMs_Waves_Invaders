// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSysten.h"

// Sets default values
AWeaponSysten::AWeaponSysten()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	maxTotalAmmo = 300;
	maxClipAmmo = 12;
	totalAmmo = 64;
	cliplAmmo = 12;
	reloadTime = 1.0f;
	fireRate = 1.0f;

	weaponType = EWeaponType::E_Rifle;
	weaponMode = EWeaponMode::E_Single;
}

void AWeaponSysten::Fire()
{
	switch (weaponMode)
	{
	case EWeaponMode::E_Single:
		FireWeapon();
		cliplAmmo -= 1;
		break;

	case EWeaponMode::E_Burst:
		FireWeapon();
		cliplAmmo -= 3;
		break;

	case EWeaponMode::E_Auto:
		FireWeapon();
		cliplAmmo -= 1;
		break;
	}
}

// Called when the game starts or when spawned
void AWeaponSysten::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponSysten::Tick(float DeltaTime)
{
	

}

