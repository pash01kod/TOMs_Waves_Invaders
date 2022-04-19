// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyTemplate.generated.h"

UCLASS()
class WAVES_INVADERS_API AEnemyTemplate : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyTemplate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") bool HasTakenDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") bool IsDead;
	
	UFUNCTION(BlueprintCallable) void TakeDamage(float _damage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
