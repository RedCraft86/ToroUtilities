// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/CurvePlayer/CurvePlayerFloat.h"

UCurvePlayerFloat::UCurvePlayerFloat()
{
	Curve.AddOrUpdatePoint(0.0f, 0.0f);
	Curve.AddOrUpdatePoint(1.0f, 1.0f);
}

void UCurvePlayerFloat::SetCurve(const FInlineFloatCurve& InCurve)
{
	Curve =	InCurve;
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}

void UCurvePlayerFloat::BroadcastEval() const
{
	const float Value = GetValue();
	OnEvaluateTick.Broadcast(Value, CurrentTime);
	OnEvaluateTickBP.Broadcast(Value, CurrentTime);
}

void UCurvePlayerFloat::BeginPlay()
{
	Super::BeginPlay();
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}
