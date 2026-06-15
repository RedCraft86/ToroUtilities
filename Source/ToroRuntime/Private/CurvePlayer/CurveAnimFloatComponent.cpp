// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "CurvePlayer/CurveAnimFloatComponent.h"

UCurveAnimFloatComponent::UCurveAnimFloatComponent()
{
	Curve.AddOrUpdatePoint(0.0f, 0.0f);
	Curve.AddOrUpdatePoint(1.0f, 1.0f);
}

void UCurveAnimFloatComponent::SetCurve(const FInlineFloatCurve& InCurve)
{
	if (InCurve.HasAnyData())
	{
		StopPlaying(false);

		Curve =	InCurve;
		InitializeCurve();

		BroadcastEval();
	}
}

void UCurveAnimFloatComponent::InitializeCurve()
{
	Curve.GetTimeRange(TimeRange.X, TimeRange.Y);
	CurrentTime = TimeRange.X;
}

void UCurveAnimFloatComponent::BroadcastEval() const
{
	const float Value = GetValue();
	OnEvaluateTick.Broadcast(Value, CurrentTime);
	OnEvaluateTickBP.Broadcast(Value, CurrentTime);
}
