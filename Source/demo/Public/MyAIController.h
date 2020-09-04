// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "MyAIController.generated.h"

/**
 *
 */
UCLASS()
class DEMO_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	void BeginPlay() override;

	AMyAIController();

	virtual void Tick(float DeltaTime) override;

	//void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	UPROPERTY()
		TArray<AActor*> Waypoints;

	UFUNCTION()
		FVector GetRandomWaypoint();

	UFUNCTION()
		void GoToRandomWaypoint();

};
