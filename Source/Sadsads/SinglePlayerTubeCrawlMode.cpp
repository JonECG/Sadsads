// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "SinglePlayerTubeCrawlMode.h"

#include "Tube.h"
#include "TubeCrawler.h"
#include "TubeObstacleManager.h"
#include "TubeCheckpointManager.h"

ASinglePlayerTubeCrawlMode::ASinglePlayerTubeCrawlMode()
{
	PrimaryActorTick.bCanEverTick = true;

	obstacleManager = CreateDefaultSubobject<UTubeObstacleManager>(TEXT("Obstacle Manager"));
	checkpointManager = CreateDefaultSubobject<UTubeCheckpointManager>(TEXT("Checkpoint Manager"));
}

// Called when the game starts or when spawned
void ASinglePlayerTubeCrawlMode::BeginPlay()
{
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

				checkpointManager->SetTube(tube);
				checkpointManager->SetCrawler(crawler);
			}
		}
	}

	currentLife = maxLife;

	// Super non-intuitive. But BP code is exectuted through a native call in Super
	Super::BeginPlay();
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

		targetSpeed = FMath::Clamp(targetSpeed + DeltaTime*acceleration, startSpeed, maxSpeed);
		crawler->SetSpeed(FMath::Clamp(crawler->GetSpeed() + DeltaTime*recoveryAcceleration, 0.f, targetSpeed));

		if (obstacleManager)
		{
			if (lastObstacleSpawnPosition < tube->GetEndOffset())
			{
				float progress = (crawler->GetSpeed() - startSpeed) / (maxSpeed - startSpeed);
				
				lastObstacleSpawnPosition += obstacleDistanceStart + (obstacleDistanceEnd-obstacleDistanceStart) * progress;
				obstacleManager->SpawnObstacle(tube, lastObstacleSpawnPosition);
			}
		}
	}
}

int ASinglePlayerTubeCrawlMode::GetScore_Implementation() const
{
	return (crawler) ? 0 : (int)crawler->GetRelativePosition().Z;
}
