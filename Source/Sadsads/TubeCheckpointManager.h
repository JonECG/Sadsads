// Copyright Jonathan Pearl 2017

#pragma once

#include "Components/ActorComponent.h"
#include "TubeCheckpointManager.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SADSADS_API UTubeCheckpointManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTubeCheckpointManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetCrawler(class ATubeCrawler* newCrawler) { crawler = newCrawler; }
	FORCEINLINE void SetTube(class ATube* newTube) { tube = newTube; }
	FORCEINLINE bool GetIsChecpointActive() const { return isCheckpointActive; }

	UFUNCTION(BlueprintCallable)
	void SetCheckpoint(float position);

	//DECLARE_EVENT_TwoParams(UTubeCheckpointManager, FCheckpointReachedEvent, AActor*, class ATubeCrawler*)
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCheckpointReachedEvent, AActor*, CheckpointActor, class ATubeCrawler*, TubeCrawler);
	FCheckpointReachedEvent& OnCheckpointReached() { return checkpointReachedEvent; }

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> checkpointRepresentation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor * currentCheckpointActor;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATubeCrawler* crawler;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATube* tube;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float tubePosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool isCheckpointActive;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FCheckpointReachedEvent checkpointReachedEvent;
};
