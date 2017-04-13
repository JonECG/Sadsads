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

	positionStart = 0.f;
	currentSegmentIndex = 0;
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
		SegmentEndpoint point;
		if (segmentEndPoints.Num() == 0)
		{
			point.center = FVector::ZeroVector;
			point.offsetAndScale = FVector(0, 0, 1);
			point.orientation = FQuat::Identity;// FMatrix(FVector(1, 0, 0), FVector(0, 1, 0), FVector(0, 0, 1), FVector::ZeroVector).ToQuat();
		}
		else
		{
			if (segmentEndPoints.Num() > numberOfSegments + 2)
			{
				positionStart += segmentLength;
				currentSegmentIndex++;
				segmentEndPoints.RemoveAt(0);
			}

			//int storedSeed = FMath::GetRandSeed();
			//FMath::RandInit(currentSeed);

			FQuat newOrientation;
			{
				// Using a random vector as the axis is able to rotate the direction up to the max rotation but favors going straight
				newOrientation = segmentEndPoints.Last().orientation * FQuat(FMath::VRand(), FMath::DegreesToRadians(FMath::FRandRange(0, maxAngleTurn)));
			}

			point.center = segmentEndPoints.Last().center + newOrientation * FVector::ForwardVector * segmentLength;
			float invScale = FMath::FRandRange(0, maxScaleChange);
			point.offsetAndScale = FVector(FMath::FRandRange(-invScale, invScale), FMath::FRandRange(-invScale, invScale), 1 - invScale);
			point.orientation = newOrientation;
			//currentSeed = FMath::GetRandSeed();
			//FMath::RandInit(storedSeed);
		}
		segmentEndPoints.Add(point);
	}
	GenerateMesh();
}

// Catmull-Rom interpolation
// Takes the 4 points and their time values and samples between the middle two points using the alpha 0-1
template <typename ValueType>
inline ValueType interpolate(ValueType* p, float* time, float alpha, ValueType (*pairInterpFunc)(ValueType, ValueType, float) = 0)
{
	float t = alpha * (time[2] - time[1]) + time[1];
	if (pairInterpFunc)
	{
		ValueType L01 = pairInterpFunc(p[0], p[1], (t - time[0]) / (time[1] - time[0]));
		ValueType L12 = pairInterpFunc(p[1], p[2], (t - time[1]) / (time[2] - time[1]));
		ValueType L23 = pairInterpFunc(p[2], p[3], (t - time[2]) / (time[3] - time[2]));
		ValueType L012 = pairInterpFunc(L01, L12, (t - time[0]) / (time[2] - time[0]));
		ValueType L123 = pairInterpFunc(L12, L23, (t - time[1]) / (time[3] - time[1]));
		ValueType C12 = pairInterpFunc(L012, L123, (t - time[1]) / (time[2] - time[1]));
		return C12;
	}
	else
	{
		ValueType L01 = p[0] * (time[1] - t) * ( 1 / (time[1] - time[0])) + p[1] * (t - time[0]) * ( 1 / (time[1] - time[0]));
		ValueType L12 = p[1] * (time[2] - t) * ( 1 / (time[2] - time[1])) + p[2] * (t - time[1]) * ( 1 / (time[2] - time[1]));
		ValueType L23 = p[2] * (time[3] - t) * ( 1 / (time[3] - time[2])) + p[3] * (t - time[2]) * ( 1 / (time[3] - time[2]));
		ValueType L012 = L01 * (time[2] - t) * ( 1 / (time[2] - time[0])) + L12  * (t - time[0]) * ( 1 / (time[2] - time[0]));
		ValueType L123 = L12 * (time[3] - t) * ( 1 / (time[3] - time[1])) + L23  * (t - time[1]) * ( 1 / (time[3] - time[1]));
		ValueType C12 = L012 * (time[2] - t) * ( 1 / (time[2] - time[1])) + L123 * (t - time[1]) * ( 1 / (time[2] - time[1]));
		return C12;
	}
}

FVector vectorInterp(FVector a, FVector b, float alpha)
{
	return a + (b - a)*alpha;
}

ATube::SegmentEndpoint pairInterp(ATube::SegmentEndpoint a, ATube::SegmentEndpoint b, float alpha)
{ 
	ATube::SegmentEndpoint result;
	result.center = FMath::Lerp(a.center, b.center, alpha);
	result.offsetAndScale = FMath::Lerp(a.offsetAndScale, b.offsetAndScale, alpha);
	result.orientation = FQuat::Slerp(a.orientation, b.orientation, alpha);
	return result;
}

ATube::SegmentEndpoint ATube::GetIntermediatePoint(float segmentAlpha) const
{
	segmentAlpha = FMath::Clamp(segmentAlpha, 1.f, numberOfSegments - 1.f);

	int segment = (int)segmentAlpha;
	float alpha = segmentAlpha - segment;

	if (alpha == 0)
		return segmentEndPoints[segment];

	float times[4];
	{
		times[0] = 0;
		for (int i = 1; i < 4; i++)
		{
			times[i] = times[i - 1] + (segmentEndPoints[segment + i - 2].center - segmentEndPoints[segment + i - 1].center).Size();
		}
	}

	SegmentEndpoint points[] = { segmentEndPoints[segment - 1], segmentEndPoints[segment], segmentEndPoints[segment + 1], segmentEndPoints[segment + 2] };

	return interpolate(points, times, alpha, &pairInterp);
}

ATube::SegmentEndpoint ATube::GetIntermediatePointFitted(float segmentAlpha) const
{
	SegmentEndpoint current = GetIntermediatePoint(segmentAlpha);

	const float GAP = 0.1f;
	SegmentEndpoint prev = GetIntermediatePoint(segmentAlpha - GAP);
	SegmentEndpoint next = GetIntermediatePoint(segmentAlpha + GAP);
	
	FVector newForward = next.center - prev.center;

	if (newForward.SizeSquared() < 0.001)
		return current;

	FVector newRight = FVector::CrossProduct(current.orientation * FVector::UpVector, newForward);
	FVector newUp = FVector::CrossProduct(newForward, newRight);
	newForward.Normalize();
	newRight.Normalize();
	newUp.Normalize();

	current.orientation = FMatrix(newForward, newRight, newUp, FVector::ZeroVector).ToQuat();
	
	return current;
}

void ATube::GetWorldOrientation(FVector relativePosition, FVector &outWorldPosition, FMatrix &outWorldRotation) const
{
	float segmentRatio = (relativePosition.Z - positionStart) / segmentLength;

	if (segmentRatio < 0 || segmentRatio >= numberOfSegments - 1)
		return;

	int segment = (int)segmentRatio;
	float ratio = segmentRatio - segment;
	segment++;
	
	SegmentEndpoint currentPoint = GetIntermediatePointFitted(segment + ratio);

	FVector dir = currentPoint.orientation * FVector::ForwardVector;
	FVector relX = currentPoint.orientation * FVector::RightVector;
	FVector relY = currentPoint.orientation * FVector::UpVector;

	outWorldPosition = currentPoint.center + (relativePosition.X * relX + relativePosition.Y * relY) * tubeRadius;
	outWorldRotation = FMatrix(relX, relY, dir, FVector::ZeroVector );
}

float ATube::GetStartOffset() const
{
	return positionStart;
}

float ATube::GetEndOffset() const
{
	return positionStart + FMath::Max(0, segmentEndPoints.Num() - 2) * segmentLength;
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
		for (int segment = 1; segment < segmentEndPoints.Num() - 2; segment++)
		{
			for (int ring = 0; ring <= numberOfRingsPerSegment; ring++)
			{
				float alpha = float(ring) / numberOfRingsPerSegment;

				SegmentEndpoint currentPoint = GetIntermediatePointFitted(segment + alpha); //segmentEndPoints[(ring > numberOfRingsPerSegment / 2) ? segment + 1 : segment];

				FVector dir = currentPoint.orientation * FVector::ForwardVector;
				FVector relX = currentPoint.orientation * FVector::RightVector;
				FVector relY = currentPoint.orientation * FVector::UpVector;

				for (int vertex = 0; vertex <= numberOfVerticesPerRing; vertex++)
				{
					float ratio = float(vertex) / numberOfVerticesPerRing;
					float angle = FMath::DegreesToRadians( 360.f * ratio);
					vertices.Add(currentPoint.center + tubeRadius * (relX * FMath::Cos(angle) + relY * FMath::Sin(angle) ));
					uvs.Add(FVector2D(uvPerSegmentRadial * ratio, -uvPerSegmentLength * (segment + currentSegmentIndex + alpha)));
				}
			}
		}
	}

	TArray<int> triangles;
	{
		int verticesPerSegment = (numberOfVerticesPerRing+1) * (numberOfRingsPerSegment+1);

		for (int segment = 1; segment < segmentEndPoints.Num() - 2; segment++)
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

ATube::SegmentEndpoint operator+(const ATube::SegmentEndpoint & lhs, const ATube::SegmentEndpoint & rhs)
{
	ATube::SegmentEndpoint result;
	result.center = lhs.center + rhs.center;
	result.offsetAndScale = lhs.offsetAndScale + rhs.offsetAndScale;
	result.orientation = lhs.orientation + rhs.orientation;
	return result;
}

ATube::SegmentEndpoint operator*(const ATube::SegmentEndpoint & lhs, float rhs)
{
	ATube::SegmentEndpoint result;
	result.center = lhs.center * rhs;
	result.offsetAndScale = lhs.offsetAndScale * rhs;
	result.orientation = lhs.orientation * rhs;
	return result;
}
