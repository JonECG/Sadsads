// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "TubeObstacle.h"

#include "Tube.h"
#include "TubeCrawler.h"

// Sets default values
ATubeObstacle::ATubeObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	isActive = false;
}

// Called when the game starts or when spawned
void ATubeObstacle::BeginPlay()
{
	Super::BeginPlay();
	
	if (!OnActorBeginOverlap.IsAlreadyBound(this, &ATubeObstacle::OnBeginOverlap))
		OnActorBeginOverlap.AddDynamic(this, &ATubeObstacle::OnBeginOverlap);
}

void ATubeObstacle::OnBeginOverlap(class AActor* thisActor, class AActor* otherActor)
{
	
	if (isActive && Cast<ATubeCrawler>(otherActor))
	{
		isActive = false;

		// TODO: Actual obstacle stuff
		thisActor->Destroy();
	}
}

bool ATubeObstacle::ShouldTickIfViewportsOnly() const
{
	return true;
}

// Called every frame
void ATubeObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugShapes();

	if (tube)
	{
		if (snapToPerimeter)
		{
			float invDist = 1 / tubePosition.Size2D();
			tubePosition.X *= invDist;
			tubePosition.Y *= invDist;

			tubeRotation = tubePosition.HeadingAngle() + 3.14159f / 2;
		}

		FVector position;
		FRotator rotation;
		FVector scale(1, 1, 1);

		tube->GetWorldOrientation(tubePosition, position, rotation);

		scale = scale * tube->GetRadius() / FMath::Lerp(minReferenceScale, maxReferenceScale, currentScaleRatio);


		rotation = (FQuat(rotation.RotateVector(FVector::ForwardVector), tubeRotation) * rotation.Quaternion()).Rotator();

		RootComponent->SetWorldScale3D(scale);
		RootComponent->SetWorldLocationAndRotation(position, rotation);

		if (tubePosition.Z < tube->GetStartOffset())
			Destroy();
	}
}

void ATubeObstacle::DrawDebugShapes() const
{
	if (!(GetWorld()->WorldType == EWorldType::EditorPreview || GetWorld()->WorldType == EWorldType::Editor))
		return;

	float maxRad = FMath::Max(minReferenceScale, maxReferenceScale);
	const float AXIS_THICKNESS = 1.f;
	if (!snapToPerimeter)
	{
		DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, 0, maxRad), FColor(0, 255, 0), false, -1, 0, AXIS_THICKNESS);
		DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(0, maxRad, 0), FColor(255, 0, 0), false, -1, 0, AXIS_THICKNESS);
		DrawDebugLine(GetWorld(), FVector(0, 0, 0), FVector(maxRad, 0, 0), FColor(0, 0, 255), false, -1, 0, AXIS_THICKNESS);
		DrawDebugCylinder(GetWorld(), FVector(-maxRad, 0, 0), FVector(maxRad, 0, 0), minReferenceScale, 32, FColor(200, 200, 200));
		DrawDebugCylinder(GetWorld(), FVector(-maxRad, 0, 0), FVector(maxRad, 0, 0), maxReferenceScale, 32, FColor(100, 100, 100));
		//DrawDebugSphere(GetWorld(), FVector(0, 0, 0), minReferenceScale, 32, FColor(0, 0, 255));
		//DrawDebugSphere(GetWorld(), FVector(0, 0, 0), maxReferenceScale, 32, FColor(0, 0, 128));
	}
	else
	{
		DrawDebugLine(GetWorld(), FVector(0, 0, maxRad), FVector(0, 0, 2 * maxRad), FColor(0, 255, 0), false, -1, 0, AXIS_THICKNESS);
		DrawDebugLine(GetWorld(), FVector(0, 0, maxRad), FVector(0, maxRad, maxRad), FColor(255, 0, 0), false, -1, 0, AXIS_THICKNESS);
		DrawDebugLine(GetWorld(), FVector(0, 0, maxRad), FVector(maxRad, 0, maxRad), FColor(0, 0, 255), false, -1, 0, AXIS_THICKNESS);
		DrawDebugCylinder(GetWorld(), FVector(-maxRad, 0, minReferenceScale), FVector(maxRad, 0, minReferenceScale), minReferenceScale, 32, FColor(200, 200, 200));
		DrawDebugCylinder(GetWorld(), FVector(-maxRad, 0, maxReferenceScale), FVector(maxRad, 0, maxReferenceScale), maxReferenceScale, 32, FColor(100, 100, 100));
		//DrawDebugSphere(GetWorld(), FVector(0, 0, minReferenceScale), minReferenceScale, 32, FColor(0, 0, 255));
		//DrawDebugSphere(GetWorld(), FVector(0, 0, maxReferenceScale), maxReferenceScale, 32, FColor(0, 0, 128));
	}
}

