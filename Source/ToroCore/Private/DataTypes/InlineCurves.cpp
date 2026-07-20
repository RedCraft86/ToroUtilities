// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#include "DataTypes/InlineCurves.h"

void FInlineFloatCurve::RemovePoint(const float Time)
{
	if (FRichCurve* CurvePtr = GetRichCurve())
	{
		const FKeyHandle Point = CurvePtr->FindKey(Time);
		if (Point.IsValid()) CurvePtr->DeleteKey(Point);
	}
}

void FInlineFloatCurve::AddOrUpdatePoint(const float Time, const float Value, const ERichCurveTangentMode Tangent)
{
	if (FRichCurve* CurvePtr = GetRichCurve())
	{
		const FKeyHandle Point = CurvePtr->UpdateOrAddKey(Time, Value);
		if (Point.IsValid()) CurvePtr->SetKeyTangentMode(Point, Tangent);
	}
}

float FInlineFloatCurve::GetValue(float InTime) const
{
	return HasAnyData() ? GetRichCurveConst()->Eval(InTime) : 0.0f;
}

void FInlineFloatCurve::GetTimeRange(float& Min, float& Max) const
{
	GetRichCurveConst()->GetTimeRange(Min, Max);
}

void FInlineFloatCurve::GetTimeRange(double& Min, double& Max) const
{
	float X, Y;
	GetTimeRange(X, Y);
	Min = X; Max = Y;
}

void FInlineFloatCurve::GetValueRange(float& Min, float& Max) const
{
	GetRichCurveConst()->GetValueRange(Min, Max);
}

void FInlineFloatCurve::GetValueRange(double& Min, double& Max) const
{
	float X, Y;
	GetValueRange(X, Y);
	Min = X; Max = Y;
}

void FInlineVectorCurve::RemovePoints(const float Time)
{
	for (int i = 0; i < Components::NUM; i++)
	{
		if (FRichCurve* CurvePtr = GetRichCurve(i))
		{
			const FKeyHandle Handle = CurvePtr->FindKey(Time);
			if (Handle.IsValid())
			{
				CurvePtr->DeleteKey(Handle);
			}
		}
	}
}

void FInlineVectorCurve::AddOrUpdatePoints(const float Time, const FVector& Value, const ERichCurveTangentMode Tangent)
{
	for (int i = 0; i < Components::NUM; i++)
	{
		if (FRichCurve* CurvePtr = GetRichCurve(i))
		{
			const FKeyHandle Point = CurvePtr->UpdateOrAddKey(Time, Value.Component(i));
			if (Point.IsValid())
			{
				CurvePtr->SetKeyTangentMode(Point, Tangent);
			}
		}
	}
}

FVector FInlineVectorCurve::GetValue(float InTime) const
{
	return HasAnyData() ? FVector {
		GetRichCurveConst(Components::X)->Eval(InTime),
		GetRichCurveConst(Components::Y)->Eval(InTime),
		GetRichCurveConst(Components::Z)->Eval(InTime)
	} : FVector::ZeroVector;
}

void FInlineVectorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3, Max1, Max2, Max3;
	
	GetRichCurveConst(Components::X)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(Components::Y)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(Components::Z)->GetTimeRange(Min3, Max3);

	Min = FMath::Min3(Min1, Min2, Min3);
	Max = FMath::Max3(Max1, Max2, Max3);
}

void FInlineVectorCurve::GetTimeRange(double& Min, double& Max) const
{
	float X, Y;
	GetTimeRange(X, Y);
	Min = X; Max = Y;
}

void FInlineVectorCurve::GetValueRange(FVector& Min, FVector& Max) const
{
	float MinX, MinY, MinZ, MaxX, MaxY, MaxZ;

	GetRichCurveConst(Components::X)->GetValueRange(MinX, MaxX);
	GetRichCurveConst(Components::Y)->GetValueRange(MinY, MaxY);
	GetRichCurveConst(Components::Z)->GetValueRange(MinZ, MaxZ);

	Min = { MinX, MinY, MinZ };
	Max = { MaxX, MaxY, MaxZ };
}

FRichCurve* FInlineColorCurve::GetRichCurve(int32 Index)
{
	if (Index < 0 || Index >= Components::NUM)
	{
		return nullptr;
	}

	if (IsValid(Curve.ExternalCurve))
	{
		return &Curve.ExternalCurve->FloatCurves[Index];
	}

	return &Curve.ColorCurves[Index];
}

const FRichCurve* FInlineColorCurve::GetRichCurveConst(int32 Index) const
{
	if (Index < 0 || Index >= Components::NUM)
	{
		return nullptr;
	}

	if (IsValid(Curve.ExternalCurve))
	{
		return &Curve.ExternalCurve->FloatCurves[Index];
	}

	return &Curve.ColorCurves[Index];
}

void FInlineColorCurve::RemovePoints(const float Time)
{
	for (int i = 0; i < Components::NUM; i++)
	{
		if (FRichCurve* CurvePtr = GetRichCurve(i))
		{
			const FKeyHandle Handle = CurvePtr->FindKey(Time);
			if (Handle.IsValid())
			{
				CurvePtr->DeleteKey(Handle);
			}
		}
	}
}

void FInlineColorCurve::AddOrUpdatePoints(const float Time, const FLinearColor& Value, const ERichCurveTangentMode Tangent)
{
	for (int i = 0; i < Components::NUM; i++)
	{
		if (FRichCurve* CurvePtr = GetRichCurve(i))
		{
			const FKeyHandle Point = CurvePtr->UpdateOrAddKey(Time, Value.Component(i));
			if (Point.IsValid())
			{
				CurvePtr->SetKeyTangentMode(Point, Tangent);
			}
		}
	}
}

FLinearColor FInlineColorCurve::GetValue(float InTime) const
{
	if (!HasAnyData())
	{
		return FLinearColor::Transparent;
	}

	FLinearColor Result {
		FMath::Max(0.0f, GetRichCurveConst(Components::R)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(Components::G)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(Components::B)->Eval(InTime))
	};

	const FRichCurve* Alpha = GetRichCurveConst(Components::A);
	Result.A = Alpha->HasAnyData() ? FMath::Max(0.0f, Alpha->Eval(InTime)) : 1.0f;

	return Result;
}

void FInlineColorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3, Min4, Max1, Max2, Max3, Max4;

	GetRichCurveConst(Components::R)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(Components::G)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(Components::B)->GetTimeRange(Min3, Max3);
	GetRichCurveConst(Components::A)->GetTimeRange(Min4, Max4);

	Min = FMath::Min3(Min1, Min2, FMath::Min(Min3, Min4));
	Max = FMath::Max3(Max1, Max2, FMath::Max(Max3, Max4));
}

void FInlineColorCurve::GetTimeRange(double& Min, double& Max) const
{
	float X, Y;
	GetTimeRange(X, Y);
	Min = X; Max = Y;
}

void FInlineColorCurve::GetValueRange(FLinearColor& Min, FLinearColor& Max) const
{
	float MinR, MinG, MinB, MinA = 1.0f, MaxR, MaxG, MaxB, MaxA = 1.0f;

	GetRichCurveConst(Components::R)->GetValueRange(MinR, MaxR);
	GetRichCurveConst(Components::G)->GetValueRange(MinG, MaxG);
	GetRichCurveConst(Components::B)->GetValueRange(MinB, MaxB);

	const FRichCurve* Alpha = GetRichCurveConst(Components::A);
	if (Alpha->HasAnyData())
	{
		Alpha->GetValueRange(MinA, MaxA);
	}

	Min = FLinearColor(MinR, MinG, MinB, MinA);
	Max = FLinearColor(MaxR, MaxG, MaxB, MaxA);
}