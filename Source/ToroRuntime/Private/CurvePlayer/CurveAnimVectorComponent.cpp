// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
		InitializeCurve();

		BroadcastEval();
	}
}

void UCurveAnimVectorComponent::InitializeCurve()
{
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}

void UCurveAnimVectorComponent::BroadcastEval() const
{
	const FVector Value = GetValue();
	OnEvaluateTick.Broadcast(Value, CurrentTime);
	OnEvaluateTickBP.Broadcast(Value, CurrentTime);
}
