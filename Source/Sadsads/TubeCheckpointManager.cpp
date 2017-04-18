// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "TubeCheckpointManager.h"

#include "TubeCrawler.h"
#include "Tube.h"

// Sets default values for this component's properties
UTubeCheckpointManager::UTubeCheckpointManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UTubeCheckpointManager::BeginPlay()
{
	Super::BeginPlay();

	currentCheckpointActor = GetWorld()->SpawnActor<AActor>(checkpointRepresentation);
	isCheckpointActive = false;
}


// Called every frame
void UTubeCheckpointManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (currentCheckpointActor && tube && crawler)
	{
		FVector position;
		FRotator rotation;

		tube->GetWorldOrientation(FVector(0,0,tubePosition), position, rotation);
		currentCheckpointActor->GetRootComponent()->SetWorldLocationAndRotation(position, rotation);

		if (isCheckpointActive && tubePosition < crawler->GetRelativePosition().Z)
		{
			isCheckpointActive = false;
			checkpointReachedEvent.Broadcast(currentCheckpointActor, crawler);
		}
	}
}

void UTubeCheckpointManager::SetCheckpoint(float position)
{
	tubePosition = position;
	isCheckpointActive = true;
}

