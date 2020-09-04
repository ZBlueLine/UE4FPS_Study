// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "SpawnEnemy.generated.h"

UCLASS()
class DEMO_API ASpawnEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle SpawnHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StSpawnAEnemy();

	void SpawnAEnemy();

	UPROPERTY(EditDefaultsOnly, Category = "Enemytype")
	TSubclassOf<class AEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Enemytype")
	float SpawnInterval = 1.f;
};
