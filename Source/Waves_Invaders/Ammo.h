// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8 
{
	E_Rifle		UMETA(DisplayName = "5.56MM"),
	E_9MM		UMETA(DisplayName= "9MM"),
	E_BigGun	UMETA(DisplayName = "BIGGUN")
};


UCLASS()
class WAVES_INVADERS_API AAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int ammoAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType ammoType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
