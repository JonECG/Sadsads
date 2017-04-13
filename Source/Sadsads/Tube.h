// Copyright Jonathan Pearl 2017

#pragma once

#include "GameFramework/Actor.h"
#include "Tube.generated.h"

UCLASS()
class SADSADS_API ATube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddSegments(int count);

	UFUNCTION(BlueprintCallable)
	void GetWorldOrientation(FVector relativePosition, FVector &outWorldPosition, FMatrix &outWorldRotation) const;

	UFUNCTION(BlueprintCallable)
	void ConstrainRelativePosition(FVector &relativePosition, float radius) const;


	UFUNCTION(BlueprintPure)
	float GetStartOffset() const;

	UFUNCTION(BlueprintPure)
	float GetEndOffset() const;

	FORCEINLINE int GetMaxNumSegments() const { return numberOfSegments; }

	FORCEINLINE class UProceduralMeshComponent* GetTubeMesh() const { return tubeMesh; }
	
	//USTRUCT()
	struct SegmentEndpoint
	{
		FVector center;
		FVector offsetAndScale;
		FQuat orientation;

	};
	friend SegmentEndpoint operator+ (const SegmentEndpoint& lhs, const SegmentEndpoint& rhs);
	friend SegmentEndpoint operator* (const SegmentEndpoint& lhs, float rhs);
private:
	//UPROPERTY(VisibleAnywhere, Category = "Tube|Debug")
	TArray<SegmentEndpoint> segmentEndPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	int numberOfSegments = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	int numberOfRingsPerSegment = 4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	int numberOfVerticesPerRing = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	int uvPerSegmentLength = 20;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	int uvPerSegmentRadial = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	float segmentLength = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	float tubeRadius = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	float maxAngleTurn = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	float maxScaleChange = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	int seed = 0;
	int currentSeed;

	float positionStart;
	int currentSegmentIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	class UProceduralMeshComponent* tubeMesh;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void GenerateMesh();

	SegmentEndpoint GetIntermediatePoint(float segmentAlpha) const;
	SegmentEndpoint GetIntermediatePointFitted(float segmentAlpha) const;

	float RelativePositionToSegmentAlpha(float relativePosition) const;
	float SegmentAlphaToRelativePosition(float segmentAlpha) const;
};
