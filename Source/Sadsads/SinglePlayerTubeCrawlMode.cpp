// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "SinglePlayerTubeCrawlMode.h"

#include "Tube.h"
#include "TubeCrawler.h"


ASinglePlayerTubeCrawlMode::ASinglePlayerTubeCrawlMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASinglePlayerTubeCrawlMode::BeginPlay()
{
	Super::BeginPlay();

	UWorld *world = GetWorld();

	if (world)
	{
		tube = world->SpawnActor<ATube>(tubeBlueprint);

		ULocalPlayer* player = GEngine->GetGamePlayer(world, 0);
		if (player)
		{
			APawn* pawn = player->PlayerController->GetPawn();
			crawler = Cast<ATubeCrawler>(pawn);
			if (crawler)
			{
				crawler->SetTube(tube);
				crawler->SetRelativePosition(FVector(0, 0, tube->GetSegmentLength()));
				crawler->SetSpeed(startSpeed);
			}
		}
	}
}

// Called every frame
void ASinglePlayerTubeCrawlMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tube && crawler)
	{
		if ((crawler->GetRelativePosition().Z - tube->GetStartOffset()) / (tube->GetEndOffset() - tube->GetStartOffset()) > 3.f / tube->GetMaxNumSegments() )
		{
			tube->AddSegments(1);
		}

		crawler->SetSpeed(FMath::Clamp(crawler->GetSpeed() + DeltaTime*acceleration, startSpeed, maxSpeed));
	}
}
