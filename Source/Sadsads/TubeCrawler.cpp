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
		relativePosition.Z += DeltaTime * speed;

		tube->ConstrainRelativePosition(relativePosition, radius);

		FVector position;
		FMatrix rotation;

		tube->GetWorldOrientation(relativePosition, position, rotation);

		RootComponent->SetWorldLocationAndRotationNoPhysics(position, rotation.Rotator());
	}
}

// Called to bind functionality to input
void ATubeCrawler::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATubeCrawler::SetTube(ATube *tube)
{
	relativePosition = FVector::ZeroVector;
	this->tube = tube;
}

void ATubeCrawler::MoveHorizontally(float influence)
{
	float delta = FApp::GetDeltaTime();
	relativePosition.X = relativePosition.X + influence *turnSpeed * delta;
}

void ATubeCrawler::MoveVertically(float influence)
{
	float delta = FApp::GetDeltaTime();
	relativePosition.Y = relativePosition.Y + influence *turnSpeed * delta;
}
