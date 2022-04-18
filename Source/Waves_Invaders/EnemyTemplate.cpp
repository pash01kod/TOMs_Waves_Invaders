// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyTemplate.h"

// Sets default values
AEnemyTemplate::AEnemyTemplate()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Health = 1.0f;
	HasTakenDamage = false;
	IsDead = false;

}

// Called when the game starts or when spawned
void AEnemyTemplate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyTemplate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyTemplate::TakeDamage(float _damage)
{
	Health -= _damage;

	if (Health <= 0.0f)
	{
		IsDead = true;
	}
	else HasTakenDamage = true;
}