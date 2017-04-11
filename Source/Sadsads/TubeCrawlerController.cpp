// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "TubeCrawlerController.h"

#include "TubeCrawler.h"


ATubeCrawlerController::ATubeCrawlerController()
{
	
}

void ATubeCrawlerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveRight", this, &ATubeCrawlerController::MoveHorizontally);
	InputComponent->BindAxis("MoveUp", this, &ATubeCrawlerController::MoveVertically);
}

void ATubeCrawlerController::MoveHorizontally(float influence)
{
	ATubeCrawler * crawler = Cast<ATubeCrawler>(GetPawn());
	if (crawler)
		crawler->MoveHorizontally(influence);
}

void ATubeCrawlerController::MoveVertically(float influence)
{
	ATubeCrawler * crawler = Cast<ATubeCrawler>(GetPawn());
	if (crawler)
		crawler->MoveVertically(influence);
}
