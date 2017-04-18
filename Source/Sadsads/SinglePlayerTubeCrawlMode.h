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

	ASinglePlayerTubeCrawlMode();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float acceleration = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float startSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxSpeed = 3000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float obstacleDistanceStart = 10000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float obstacleDistanceEnd = 5000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float checkpointTime = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float score;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATube> tubeBlueprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ATube *tube;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class ATubeCrawler *crawler;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTubeObstacleManager *obstacleManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTubeCheckpointManager *checkpointManager;

	float lastObstacleSpawnPosition;
};
