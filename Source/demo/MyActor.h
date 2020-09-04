// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "demoProjectile.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/BoxComponent.h"
#include "MyActor.generated.h"

UCLASS()
class DEMO_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

private:
	float RunTime;

	UFUNCTION(BlueprintCallable, Category = "Damage")
		void CalculateValues();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		UShapeComponent* Root;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MyMesh;

	UPROPERTY(EditAnywhere)
		float sum;

	UPROPERTY(EditAnywhere)
		float inSeconds;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		float perSeconds;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AdemoProjectile> ProjectileClass;

	float time;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

};
