// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "TubeObstacleManager.h"

#include "TubeObstacle.h"

// Sets default values for this component's properties
UTubeObstacleManager::UTubeObstacleManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTubeObstacleManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTubeObstacleManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ATubeObstacle * UTubeObstacleManager::SpawnObstacle(ATube * tube, float position)
{
	ATubeObstacle* result = GetWorld()->SpawnActor<ATubeObstacle>(obstacleBPs[FMath::RandRange(0, obstacleBPs.Num() - 1)]);
	result->SetTube(tube);
	float positionAngle = FMath::SRand() * 3.14159f * 2;
	float positionDist = FMath::SRand();
	result->SetTubePosition(FVector(FMath::Cos(positionAngle) * positionDist, FMath::Sin(positionAngle) * positionDist,position));
	result->SetTubeRotation(FMath::SRand() * 3.14159f * 2);
	result->SetCurrentScaleRatio(FMath::SRand());
	result->SetIsActive(true);
	return result;
}

