// Copyright Jonathan Pearl 2017

#pragma once

#include "GameFramework/PlayerController.h"
#include "TubeCrawlerController.generated.h"

/**
 * 
 */
UCLASS()
class SADSADS_API ATubeCrawlerController : public APlayerController
{
	GENERATED_BODY()


	ATubeCrawlerController();

	virtual void SetupInputComponent() override;

public:
	void MoveHorizontally(float influence);
	void MoveVertically(float influence);
};
