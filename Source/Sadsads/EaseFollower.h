// Copyright Jonathan Pearl 2017

#pragma once

#include "Components/SceneComponent.h"
#include "EaseFollower.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SADSADS_API UEaseFollower : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEaseFollower();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follower")
	float minDistance = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follower")
	float maxDistance = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follower")
	float easeRatio = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follower")
	class USceneComponent *targetTransform;

	FVector previousPosition;
};
