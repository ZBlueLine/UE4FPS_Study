// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "AIController.h"

AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
}

FVector AMyAIController::GetRandomWaypoint()
{
	//APlayerController *myPlayer1 = UGameplayStatics::GetPlayerController(GetWorld(), 1);
	FVector myPlayer1 = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation();
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, myPlayer1.ToString(), true);
	FVector myPlayer = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	return myPlayer1;
}

void AMyAIController::GoToRandomWaypoint()
{
	//ATargetPoint *ans = GetRandomWaypoint();
	FVector PlayerLocation = GetRandomWaypoint();
	FVector EnemyLocation = GetCharacter()->GetActorLocation();
	FVector EnemyDirect = PlayerLocation - EnemyLocation;

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, EnemyLocation.ToString(), true);*/
	this->GetCharacter()->FaceRotation(EnemyDirect.Rotation(), 0.2);
	MoveToLocation(PlayerLocation, 5);
	//MoveToActor(ans);
}

//void AMyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
//{
//	Super::OnMoveCompleted(RequestID, Result);
//	GoToRandomWaypoint();
//}

void AMyAIController::Tick(float DeltaTime)
{
	GoToRandomWaypoint();
}