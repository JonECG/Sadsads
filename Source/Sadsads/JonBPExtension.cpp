// Copyright Jonathan Pearl 2017

#include "Sadsads.h"
#include "JonBPExtension.h"

FRotator UJonBPExtension::LookAtWithUp(const FVector& from, const FVector& to, const FVector& up)
{
	FVector forward = to - from;
	FVector right = FVector::CrossProduct(up, forward);
	FVector upBasis = FVector::CrossProduct(forward, right);
	forward.Normalize();
	right.Normalize();
	upBasis.Normalize();
	return FMatrix(forward, right, upBasis, FVector::ZeroVector).Rotator();
}
