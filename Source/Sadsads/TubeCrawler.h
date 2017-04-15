// Copyright Jonathan Pearl 2017

#pragma once

#include "GameFramework/Pawn.h"
#include "TubeCrawler.generated.h"

UCLASS()
class SADSADS_API ATubeCrawler : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this pawn's properties
	ATubeCrawler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetTube(ATube* newTube);

	UFUNCTION(BlueprintCallable)
	void MoveHorizontally(float influence);

	UFUNCTION(BlueprintCallable)
	void MoveVertically(float influence);

	FORCEINLINE FVector GetRelativePosition() const { return relativePosition; }
	FORCEINLINE void SetRelativePosition(const FVector& position) { relativePosition = position; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TubeCrawler", meta = (AllowPrivateAccess = "true"))
	class ATube* tube;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TubeCrawler", meta = (AllowPrivateAccess = "true"))
	float speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TubeCrawler", meta = (AllowPrivateAccess = "true"))
	float turnSpeed = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TubeCrawler", meta = (AllowPrivateAccess = "true"))
	FVector relativePosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TubeCrawler", meta = (AllowPrivateAccess = "true"))
	float radius = 100.f;
};
