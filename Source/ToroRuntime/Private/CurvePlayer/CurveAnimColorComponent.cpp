// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "CurvePlayer/CurveAnimColorComponent.h"

UCurveAnimColorComponent::UCurveAnimColorComponent()
{
	Curve.AddOrUpdatePoints(0.0f, FLinearColor::Black);
	Curve.AddOrUpdatePoints(1.0f, FLinearColor::White);
}

void UCurveAnimColorComponent::SetCurve(const FInlineColorCurve& InCurve)
{
	if (InCurve.HasAnyData())
	{
		StopPlaying(false);

		Curve =	InCurve;
		InitializeCurve();

		BroadcastEval();
	}
}

void UCurveAnimColorComponent::InitializeCurve()
{
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}

void UCurveAnimColorComponent::BroadcastEval() const
{
	const FLinearColor Value = GetValue();
	OnEvaluateTick.Broadcast(Value, CurrentTime);
	OnEvaluateTickBP.Broadcast(Value, CurrentTime);
}
