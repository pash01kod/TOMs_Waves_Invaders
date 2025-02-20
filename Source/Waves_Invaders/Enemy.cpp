// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EnemyTexture = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Archer"));
	Health = 1.0f;
	HasTakenDamage = false;
	IsDead = false;
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> CubeVisualAsset(TEXT("/Game/CityofBrass_Enemies/Meshes/Enemy/Archer/Corpse_Skeleton.Corpse_Skeleton"));


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::TakeDamage(float _damage)
{
	Health -= _damage;
	
	if (Health <= 0.0f)
	{
		IsDead = true;
	}
	else HasTakenDamage = true;
}



