// Copyright Jonathan Pearl 2017

#pragma once

#include "SadsadsGameModeBase.h"
#include "SinglePlayerTubeCrawlMode.generated.h"

/**
 * 
 */
UCLASS()
class SADSADS_API ASinglePlayerTubeCrawlMode : public ASadsadsGameModeBase
{
	GENERATED_BODY()
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float acceleration;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float score;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATube> tubeBlueprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ATube *tube;
};
