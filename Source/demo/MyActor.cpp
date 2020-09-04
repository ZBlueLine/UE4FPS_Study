// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "Engine/World.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<UBoxComponent>(TEXT("Root"));
	RootComponent = Root;

	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	MyMesh->AttachTo(RootComponent);
	sum = 100;
	inSeconds = 40;
}

void AMyActor::CalculateValues()
{
	if(inSeconds!=0)perSeconds = sum / inSeconds;
}

#if WITH_EDITOR
void AMyActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	CalculateValues();

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	time += DeltaTime;
	if (time > 2)
	{
		time = 0;

		UWorld* const World = GetWorld();
		FActorSpawnParameters SpawnInfo;

		AdemoProjectile* MyHUD;
		MyHUD = GetWorld()->SpawnActor<AdemoProjectile>(ProjectileClass, SpawnInfo);
		MyHUD->SetActorLocation(FVector(0, -90, 260));
	}

}

