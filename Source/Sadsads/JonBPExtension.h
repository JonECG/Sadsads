// Copyright Jonathan Pearl 2017

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "JonBPExtension.generated.h"

/**
 * 
 */
UCLASS()
class SADSADS_API UJonBPExtension : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure)
	static	FRotator LookAtWithUp(const FVector& from,const FVector& to,const FVector& up);
};
