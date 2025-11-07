// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/CurvePlayer/CurvePlayerVector.h"

UCurvePlayerVector::UCurvePlayerVector()
{
	Curve.AddOrUpdatePoints(0.0f, FVector::ZeroVector);
	Curve.AddOrUpdatePoints(1.0f, FVector::OneVector);
}

void UCurvePlayerVector::SetCurve(const FInlineVectorCurve& InCurve)
{
	Curve =	InCurve;
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}

void UCurvePlayerVector::BroadcastEval() const
{
	const FVector Value = GetValue();
	OnEvaluateTick.Broadcast(Value, CurrentTime);
	OnEvaluateTickBP.Broadcast(Value, CurrentTime);
}

void UCurvePlayerVector::BeginPlay()
{
	Super::BeginPlay();
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}
