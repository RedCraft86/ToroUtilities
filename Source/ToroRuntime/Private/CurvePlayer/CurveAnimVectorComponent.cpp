// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "CurvePlayer/CurveAnimVectorComponent.h"

UCurveAnimVectorComponent::UCurveAnimVectorComponent()
{
	Curve.AddOrUpdatePoints(0.0f, FVector::ZeroVector);
	Curve.AddOrUpdatePoints(1.0f, FVector::OneVector);
}

void UCurveAnimVectorComponent::SetCurve(const FInlineVectorCurve& InCurve)
{
	if (InCurve.HasAnyData())
	{
		StopPlaying(false);

		Curve =	InCurve;
		Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
		CurrentTime = TimeRange.X;

		BroadcastEval();
	}
}

void UCurveAnimVectorComponent::BroadcastEval() const
{
	const FVector Value = GetValue();
	OnEvaluateTick.Broadcast(Value, CurrentTime);
	OnEvaluateTickBP.Broadcast(Value, CurrentTime);
}

void UCurveAnimVectorComponent::BeginPlay()
{
	Super::BeginPlay();
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}
