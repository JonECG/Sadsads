// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "SinglePlayerTubeCrawlMode.h"

#include "Tube.h"
#include "TubeCrawler.h"


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
			APawn* pawn = player->PlayerController->GetControlledPawn();
			ATubeCrawler* crawler = Cast<ATubeCrawler>(pawn);
			if (crawler)
			{
				crawler->SetTube(tube);
			}
		}
	}
}
