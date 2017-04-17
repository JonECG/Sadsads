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
		
protected:
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class ATubeObstacle>> obstacleBPs;
};
