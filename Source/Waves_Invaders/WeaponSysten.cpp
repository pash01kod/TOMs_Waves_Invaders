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

