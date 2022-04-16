// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponSysten.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_Rifle		UMETA(DisplayName = "5.56MM"),
	E_9MM		UMETA(DisplayName = "9MM"),
	E_BigGun	UMETA(DisplayName = "BIGGUN")
};

UCLASS()
class WAVES_INVADERS_API AWeaponSysten : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponSysten();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
	void FireWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool isObtained;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int maxTotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int maxClipAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int totalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int cliplAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float reloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType weaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float fireRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString name;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

};
