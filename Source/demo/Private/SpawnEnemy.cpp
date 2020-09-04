// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEnemy.h"

// Sets default values
ASpawnEnemy::ASpawnEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ASpawnEnemy::BeginPlay()
{
	Super::BeginPlay();

	StSpawnAEnemy();
}

// Called every frame
void ASpawnEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnEnemy::SpawnAEnemy()
{
	FVector Thislocation = this->GetActorLocation();
	FRotator ThisRotator = this->GetActorRotation();
	AEnemy *NewBot = GetWorld()->SpawnActor<AEnemy>(EnemyClass, Thislocation, ThisRotator);
}

void ASpawnEnemy::StSpawnAEnemy()
{
	GetWorldTimerManager().SetTimer(SpawnHandle, this, &ASpawnEnemy::SpawnAEnemy, SpawnInterval, true);
}