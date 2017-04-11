// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "EaseFollower.h"


// Sets default values for this component's properties
UEaseFollower::UEaseFollower()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEaseFollower::BeginPlay()
{
	Super::BeginPlay();


	previousPosition = GetComponentTransform().GetLocation();
	
}


// Called every frame
void UEaseFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (targetTransform)
	{
		FVector to = targetTransform->GetComponentTransform().GetLocation();
		FVector from = previousPosition;
		FVector newPosition = ((to * easeRatio) + from) / (easeRatio + 1);

		if (maxDistance >= 0 || minDistance >= 0)
		{
			FVector diff = newPosition - to;
			float dist = diff.Size();
			float newDist = dist;
			if (maxDistance >= 0)
				newDist = FMath::Min(newDist, maxDistance);
			if (minDistance >= 0)
				newDist = FMath::Max(newDist, minDistance);

			newPosition = to + diff * newDist / dist;
		}

		SetWorldLocation(newPosition);
		previousPosition = newPosition;
	}
}

