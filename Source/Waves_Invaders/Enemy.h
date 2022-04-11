// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class WAVES_INVADERS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") float Health; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyStats") float Speed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemyMesh") USkeletalMeshComponent *EnemyTexture;
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable) void TakeDamage(float _damage);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

