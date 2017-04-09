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

	FORCEINLINE class UProceduralMeshComponent* GetTubeMesh() const { return tubeMesh; }
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Tube|Debug")
	TArray<FVector> segmentPoints;
	UPROPERTY(VisibleAnywhere, Category = "Tube|Debug")
	TArray<FVector> segmentOffset;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tube", meta = (AllowPrivateAccess = "true"))
	class UProceduralMeshComponent* tubeMesh;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void GenerateMesh();
};
