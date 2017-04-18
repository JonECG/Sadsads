// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "TubeCrawler.h"

#include "Tube.h"


// Sets default values
ATubeCrawler::ATubeCrawler()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATubeCrawler::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATubeCrawler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tube)
	{
		if( !((APlayerController*)this->GetController())->IsInputKeyDown(EKeys::SpaceBar) )
			relativePosition.Z += DeltaTime * speed;

		float size = targetOffsetSpeed.Size();
		if (size > 1)
			targetOffsetSpeed /= size;

		float tween = 25;
		currentOffsetSpeed = (targetOffsetSpeed + currentOffsetSpeed*tween) / (tween+1);

		relativePosition += FVector(currentOffsetSpeed * turnSpeed * DeltaTime, 0);

		tube->ConstrainRelativePosition(relativePosition, radius);

		FVector position;
		FRotator rotation;

		tube->GetWorldOrientation(relativePosition, position, rotation);

		float offsetWorldSpeed = currentOffsetSpeed.Size() * (turnSpeed * tube->GetRadius());
		float angle = 90 * offsetWorldSpeed / (speed + offsetWorldSpeed);
		rotation = ( rotation.Quaternion() * FQuat(FVector(0, -currentOffsetSpeed.Y, currentOffsetSpeed.X).GetSafeNormal(), FMath::DegreesToRadians( angle ))).Rotator();
		RootComponent->SetWorldLocationAndRotation(position, rotation);
	}
}

// Called to bind functionality to input
void ATubeCrawler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATubeCrawler::SetTube(ATube *newTube)
{
	relativePosition = FVector::ZeroVector;
	this->tube = newTube;
}

void ATubeCrawler::MoveHorizontally(float influence)
{
	targetOffsetSpeed.X = influence;
}

void ATubeCrawler::MoveVertically(float influence)
{
	targetOffsetSpeed.Y = influence;
}
