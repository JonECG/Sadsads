// Copyright Jonathan Pearl 2017

#pragma once

#include "GameFramework/Actor.h"
#include "TubeObstacle.generated.h"

UCLASS()
class SADSADS_API ATubeObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATubeObstacle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual bool ShouldTickIfViewportsOnly() const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class ATube* GetTube() const { return tube; }
	FORCEINLINE void SetTube(class ATube* newTube ) { tube = newTube; }

	FORCEINLINE FVector GetTubePosition() const { return tubePosition; }
	FORCEINLINE void SetTubePosition(const FVector& newTubePosition) { tubePosition = newTubePosition; }

	FORCEINLINE float GetTubeRotation() const { return tubeRotation; }
	FORCEINLINE void SetTubeRotation(float newTubeRotation) { tubeRotation = newTubeRotation; }

	FORCEINLINE float GetCurrentScaleRatio() const { return currentScaleRatio; }
	FORCEINLINE void SetCurrentScaleRatio(float newScaleRatio) { currentScaleRatio = newScaleRatio; }

	FORCEINLINE bool GetIsActive() const { return isActive; }
	FORCEINLINE void SetIsActive(bool newIsActive) { isActive = newIsActive; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATube* tube;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector tubePosition;
	// Only affects obstacle if it's not snapped to the perimeter
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float tubeRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float currentScaleRatio;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isActive;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float minReferenceScale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float maxReferenceScale;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool snapToPerimeter;

private:
	void DrawDebugShapes() const;
	UFUNCTION()
	void OnBeginOverlap(class AActor* thisActor, class AActor* otherActor);
};
