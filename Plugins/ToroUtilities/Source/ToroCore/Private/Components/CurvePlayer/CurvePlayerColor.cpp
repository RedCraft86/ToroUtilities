// Copyright (C) RedCraft86. All Rights Reserved.

#include "Components/CurvePlayer/CurvePlayerColor.h"

UCurvePlayerColor::UCurvePlayerColor()
{
	Curve.AddOrUpdatePoints(0.0f, FLinearColor::Black);
	Curve.AddOrUpdatePoints(1.0f, FLinearColor::White);
}

void UCurvePlayerColor::SetCurve(const FInlineColorCurve& InCurve)
{
	Curve =	InCurve;
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}

void UCurvePlayerColor::BroadcastEval() const
{
	const FLinearColor Value = GetValue();
	OnEvaluateTick.Broadcast(Value, CurrentTime);
	OnEvaluateTickBP.Broadcast(Value, CurrentTime);
}

void UCurvePlayerColor::BeginPlay()
{
	Super::BeginPlay();
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}
