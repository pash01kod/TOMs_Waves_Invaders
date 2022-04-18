// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E_Rifle		UMETA(DisplayName = "5.56MM"),
	E_9MM		UMETA(DisplayName = "9MM"),
	E_BigGun	UMETA(DisplayName = "BIGGUN")
};

UCLASS()
class WAVES_INVADERS_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
