// Copyright Jonathan Pearl 2017

#pragma once

#include "Components/ActorComponent.h"
#include "TubeObstacleManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SADSADS_API UTubeObstacleManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTubeObstacleManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	class ATubeObstacle* SpawnObstacle(class ATube* tube, float position);

	void BroadcastCollision(class ATubeObstacle* obstacle, class ATubeCrawler* crawler);

	//DECLARE_EVENT_TwoParams(UTubeObstacleManager, FObstacleHitEvent, class ATubeObstacle*, class ATubeCrawler*)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FObstacleHitEvent, class ATubeObstacle*, TubeObstacle, class ATubeCrawler*, TubeCrawler);
	FObstacleHitEvent& OnObstacleHit() { return obstacleHitEvent; }
		
protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ATubeObstacle>> obstacleBPs;

	UPROPERTY(BlueprintAssignable)
	FObstacleHitEvent obstacleHitEvent;
};
