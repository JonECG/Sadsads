// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "Tube.h"


#include "KismetProceduralMeshLibrary.h"


// Sets default values
ATube::ATube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	tubeMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Tube Mesh"));
	RootComponent = tubeMesh;

	currentSeed = seed;
}

// Called when the game starts or when spawned
void ATube::BeginPlay()
{
	Super::BeginPlay();

	AddSegments(numberOfSegments + 2);
}

// Called every frame
void ATube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATube::AddSegments(int count)
{
	for (int i = 0; i < count; i++)
	{
		if (segmentPoints.Num() == 0)
		{
			segmentPoints.Add(FVector(0, 0, 0));
			segmentOffset.Add(FVector(0, 0, 1));
			continue;
		}

		if (segmentPoints.Num() == 1)
		{
			segmentPoints.Add(segmentPoints.Last() + FVector(segmentLength, 0, 0));
			segmentOffset.Add(FVector(0, 0, 1));
			continue;
		}

		if (segmentPoints.Num() > numberOfSegments + 2)
		{
			positionStart += segmentLength;
			segmentPoints.RemoveAt(0);
			segmentOffset.RemoveAt(0);
		}

		//int storedSeed = FMath::GetRandSeed();
		//FMath::RandInit(currentSeed);

		FVector newDirection;
		{
			newDirection = segmentPoints.Last() - segmentPoints.Last(1);
			// Using a random vector as the axis is able to rotate the direction up to the max rotation but favors going straight
			newDirection = newDirection.RotateAngleAxis(FMath::FRandRange(0, maxAngleTurn), FMath::VRand());
			newDirection.Normalize();
		}

		segmentPoints.Add(segmentPoints.Last() + newDirection * segmentLength);
		float invScale = FMath::FRandRange(0, maxScaleChange);
		segmentOffset.Add(FVector(FMath::FRandRange(-invScale, invScale), FMath::FRandRange(-invScale, invScale), 1 - invScale));

		//currentSeed = FMath::GetRandSeed();
		//FMath::RandInit(storedSeed);
	}
	GenerateMesh();
}

// Catmull-Rom interpolation
// Takes the 4 points and their time values and samples between the middle two points using the alpha 0-1
template <typename ValueType>
inline ValueType interpolate(ValueType* p, float* time, float alpha)
{
	float t = alpha * (time[2] - time[1]) + time[1];
	ValueType L01 = p[0] * (time[1] - t) / (time[1] - time[0]) + p[1] * (t - time[0]) / (time[1] - time[0]);
	ValueType L12 = p[1] * (time[2] - t) / (time[2] - time[1]) + p[2] * (t - time[1]) / (time[2] - time[1]);
	ValueType L23 = p[2] * (time[3] - t) / (time[3] - time[2]) + p[3] * (t - time[2]) / (time[3] - time[2]);
	ValueType L012 = L01 * (time[2] - t) / (time[2] - time[0]) + L12 * (t - time[0]) / (time[2] - time[0]);
	ValueType L123 = L12 * (time[3] - t) / (time[3] - time[1]) + L23 * (t - time[1]) / (time[3] - time[1]);
	ValueType C12 = L012 * (time[2] - t) / (time[2] - time[1]) + L123 * (t - time[1]) / (time[2] - time[1]);
	return alpha * (p[2] - p[1]) + p[1];//C12;
}

void ATube::GetWorldOrientation(FVector relativePosition, FVector &outWorldPosition, FMatrix &outWorldRotation) const
{
	float segmentRatio = (relativePosition.Z - positionStart) / segmentLength;

	if (segmentRatio < 0 || segmentRatio >= numberOfSegments - 1)
		return;

	int segment = (int)segmentRatio;
	float ratio = segmentRatio - segment;
	segment++;
	
	float times[4];
	{
		times[0] = 0;
		for (int i = 1; i < 4; i++)
		{
			times[i] = times[i] + (segmentPoints[segment + i - 2] - segmentPoints[segment + i - 1]).Size();
		}
	}

	FVector fromNormal = segmentPoints[segment + 1] - segmentPoints[segment - 1];
	FVector toNormal = segmentPoints[segment + 2] - segmentPoints[segment];

	FVector center = interpolate(&segmentPoints[segment - 1], times, ratio);

	FVector dir = FMath::Lerp(fromNormal, toNormal, ratio);
	FVector relX = FVector::CrossProduct(FVector::UpVector, dir);
	FVector relY = FVector::CrossProduct(dir, relX);
	dir.Normalize();
	relX.Normalize();
	relY.Normalize();

	outWorldPosition = center + (relativePosition.X * relX + relativePosition.Y * relY) * tubeRadius;
	outWorldRotation = FMatrix(relX, relY, dir, FVector::ZeroVector );
}

float ATube::GetStartOffset() const
{
	return positionStart;
}

float ATube::GetEndOffset() const
{
	return positionStart + FMath::Max(0, segmentPoints.Num() - 2) * segmentLength;
}

void ATube::GenerateMesh()
{
	/**
	*	Create/replace a section for this procedural mesh component.
	*	@param	SectionIndex		Index of the section to create or replace.
	*	@param	Vertices			Vertex buffer of all vertex positions to use for this mesh section.
	*	@param	Triangles			Index buffer indicating which vertices make up each triangle. Length must be a multiple of 3.
	*	@param	Normals				Optional array of normal vectors for each vertex. If supplied, must be same length as Vertices array.
	*	@param	UV0					Optional array of texture co-ordinates for each vertex. If supplied, must be same length as Vertices array.
	*	@param	VertexColors		Optional array of colors for each vertex. If supplied, must be same length as Vertices array.
	*	@param	Tangents			Optional array of tangent vector for each vertex. If supplied, must be same length as Vertices array.
	*	@param	bCreateCollision	Indicates whether collision should be created for this section. This adds significant cost.
	*/
	//UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh", meta = (AutoCreateRefTerm = "Normals,UV0,VertexColors,Tangents"))
	//	void CreateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
	// const TArray<FVector2D>& UV0, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents, bool bCreateCollision);

	TArray<FVector> vertices;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	{
		for (int segment = 1; segment < segmentPoints.Num() - 2; segment++)
		{

			float times[4];
			{
				times[0] = 0;
				for (int i = 1; i < 4; i++)
				{
					times[i] = times[i] + (segmentPoints[segment + i - 2] - segmentPoints[segment + i - 1]).Size();
				}
			}

			FVector fromNormal = segmentPoints[segment + 1] - segmentPoints[segment - 1];
			FVector toNormal = segmentPoints[segment + 2] - segmentPoints[segment];

			//FVector prev;
			for (int ring = 0; ring <= numberOfRingsPerSegment; ring++)
			{
				float alpha = float(ring) / numberOfRingsPerSegment;
				FVector center = interpolate(&segmentPoints[segment - 1], times, alpha);

				//if (ring > 0)
				{
					FVector dir = FMath::Lerp(fromNormal, toNormal, alpha);// center - prev;
					FVector relX = FVector::CrossProduct(dir, FVector::UpVector);
					FVector relY = FVector::CrossProduct(dir, relX);
					relX.Normalize();
					relY.Normalize();
					for (int vertex = 0; vertex <= numberOfVerticesPerRing; vertex++)
					{
						float angle = FMath::DegreesToRadians( 360.f * vertex / numberOfVerticesPerRing);
						vertices.Add(center + tubeRadius * (relX * FMath::Cos(angle) + relY * FMath::Sin(angle) ));
					}
				}

				//prev = center;
			}
		}
	}

	TArray<int> triangles;
	{
		int verticesPerSegment = (numberOfVerticesPerRing+1) * (numberOfRingsPerSegment+1);

		for (int segment = 1; segment < segmentPoints.Num() - 2; segment++)
		{
			for (int ring = 0; ring < numberOfRingsPerSegment; ring++)
			{
				for (int vertex = 0; vertex <= numberOfVerticesPerRing; vertex++)
				{
					// The quad
					int v0 = verticesPerSegment * (segment - 1) + ring * (numberOfVerticesPerRing + 1) + vertex;
					int v1 = verticesPerSegment * (segment - 1) + ring * (numberOfVerticesPerRing + 1) + vertex + 1;
					int v2 = verticesPerSegment * (segment - 1) + (ring + 1) * (numberOfVerticesPerRing + 1) + vertex;
					int v3 = verticesPerSegment * (segment - 1) + (ring + 1) * (numberOfVerticesPerRing + 1) + vertex + 1;

					triangles.Add(v0); triangles.Add(v1); triangles.Add(v2);
					triangles.Add(v2); triangles.Add(v1); triangles.Add(v3);
				}
			}
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("Generated mesh with %d vertices"), vertices.Num());
	tubeMesh->CreateMeshSection_LinearColor(1, vertices, triangles, normals, uvs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), false);

}

