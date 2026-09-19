// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "DataTypes/InlineCurves.h"

FRichCurve* FInlineFloatCurve::GetRichCurve()
{
	return Curve.GetRichCurve();
}

const FRichCurve* FInlineFloatCurve::GetRichCurve() const
{
	return Curve.GetRichCurveConst();
}

void FInlineFloatCurve::RemovePoint(const float Time)
{
	if (FRichCurve* CurvePtr = GetRichCurve())
	{
		const FKeyHandle Point = CurvePtr->FindKey(Time);
		if (Point.IsValid())
		{
			CurvePtr->DeleteKey(Point);
		}
	}
}

void FInlineFloatCurve::AddOrUpdatePoint(const float Time, const float Value, const ERichCurveTangentMode Tangent)
{
	if (FRichCurve* CurvePtr = GetRichCurve())
	{
		const FKeyHandle Point = CurvePtr->UpdateOrAddKey(Time, Value);
		if (Point.IsValid())
		{
			CurvePtr->SetKeyTangentMode(Point, Tangent);
		}
	}
}

float FInlineFloatCurve::GetValue(const float InTime) const
{
	return GetRichCurve()->Eval(InTime);
}

void FInlineFloatCurve::GetTimeRange(float& Min, float& Max) const
{
	GetRichCurve()->GetTimeRange(Min, Max);
}

void FInlineFloatCurve::GetValueRange(float& Min, float& Max) const
{
	GetRichCurve()->GetValueRange(Min, Max);
}

FRichCurve* FInlineVectorCurve::GetRichCurve(int32 Index)
{
	return Curve.GetRichCurve(Index);
}

const FRichCurve* FInlineVectorCurve::GetRichCurve(int32 Index) const
{
	return Curve.GetRichCurveConst(Index);
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

FVector FInlineVectorCurve::GetValue(const float InTime) const
{
	return FVector {
		GetRichCurve(Components::X)->Eval(InTime),
		GetRichCurve(Components::Y)->Eval(InTime),
		GetRichCurve(Components::Z)->Eval(InTime)
	};
}

void FInlineVectorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3, Max1, Max2, Max3;

	GetRichCurve(Components::X)->GetTimeRange(Min1, Max1);
	GetRichCurve(Components::Y)->GetTimeRange(Min2, Max2);
	GetRichCurve(Components::Z)->GetTimeRange(Min3, Max3);

	Min = FMath::Min3(Min1, Min2, Min3);
	Max = FMath::Max3(Max1, Max2, Max3);
}

void FInlineVectorCurve::GetValueRange(FVector& Min, FVector& Max) const
{
	float MinX, MinY, MinZ, MaxX, MaxY, MaxZ;

	GetRichCurve(Components::X)->GetValueRange(MinX, MaxX);
	GetRichCurve(Components::Y)->GetValueRange(MinY, MaxY);
	GetRichCurve(Components::Z)->GetValueRange(MinZ, MaxZ);

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

const FRichCurve* FInlineColorCurve::GetRichCurve(int32 Index) const
{
	return const_cast<FInlineColorCurve*>(this)->GetRichCurve(Index);
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

FLinearColor FInlineColorCurve::GetValue(const float InTime) const
{
	FLinearColor Result {
		FMath::Max(0.0f, GetRichCurve(Components::R)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurve(Components::G)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurve(Components::B)->Eval(InTime))
	};

	const FRichCurve* Alpha = GetRichCurve(Components::A);
	Result.A = Alpha->HasAnyData() ? FMath::Max(0.0f, Alpha->Eval(InTime)) : 1.0f;

	return Result;
}

void FInlineColorCurve::GetTimeRange(float& Min, float& Max) const
{
	float MinR, MinG, MinB, MinA = 1.0f, MaxR, MaxG, MaxB, MaxA = 1.0f;

	GetRichCurve(Components::R)->GetTimeRange(MinR, MaxR);
	GetRichCurve(Components::G)->GetTimeRange(MinG, MaxG);
	GetRichCurve(Components::B)->GetTimeRange(MinB, MaxB);

	const FRichCurve* Alpha = GetRichCurve(Components::A);
	if (Alpha->HasAnyData())
	{
		Alpha->GetTimeRange(MinA, MaxA);
	}

	Min = FMath::Min3(MinR, MinG, FMath::Min(MinB, MinA));
	Max = FMath::Max3(MaxR, MaxG, FMath::Max(MaxB, MaxA));
}

void FInlineColorCurve::GetValueRange(FLinearColor& Min, FLinearColor& Max) const
{
	float MinR, MinG, MinB, MinA = 1.0f, MaxR, MaxG, MaxB, MaxA = 1.0f;

	GetRichCurve(Components::R)->GetValueRange(MinR, MaxR);
	GetRichCurve(Components::G)->GetValueRange(MinG, MaxG);
	GetRichCurve(Components::B)->GetValueRange(MinB, MaxB);

	const FRichCurve* Alpha = GetRichCurve(Components::A);
	if (Alpha->HasAnyData())
	{
		Alpha->GetValueRange(MinA, MaxA);
	}

	Min = FLinearColor(MinR, MinG, MinB, MinA);
	Max = FLinearColor(MaxR, MaxG, MaxB, MaxA);
}

bool UInlineCurveLibrary::HasInlineCurveData_Float(const FInlineFloatCurve& Target)
{
	return Target.HasAnyData();
}

UCurveFloat* UInlineCurveLibrary::GetInlineCurveAsset_Float(const FInlineFloatCurve& Target)
{
	return Target.GetCurveAsset();
}

float UInlineCurveLibrary::GetInlineCurveValue_Float(const FInlineFloatCurve& Target, const float InTime)
{
	return Target.GetValue(InTime);
}

void UInlineCurveLibrary::GetInlineCurveTimeRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max)
{
	return Target.GetTimeRange(Min, Max);
}

void UInlineCurveLibrary::GetInlineCurveValueRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max)
{
	return Target.GetValueRange(Min, Max);
}

bool UInlineCurveLibrary::HasInlineCurveData_Vector(const FInlineVectorCurve& Target)
{
	return Target.HasAnyData();
}

UCurveVector* UInlineCurveLibrary::GetInlineCurveAsset_Vector(const FInlineVectorCurve& Target)
{
	return Target.GetCurveAsset();
}

FVector UInlineCurveLibrary::GetInlineCurveValue_Vector(const FInlineVectorCurve& Target, const float InTime)
{
	return Target.GetValue(InTime);
}

void UInlineCurveLibrary::GetInlineCurveTimeRange_Vector(const FInlineVectorCurve& Target, float& Min, float& Max)
{
	return Target.GetTimeRange(Min, Max);
}

void UInlineCurveLibrary::GetInlineCurveValueRange_Vector(const FInlineVectorCurve& Target, FVector& Min, FVector& Max)
{
	return Target.GetValueRange(Min, Max);
}

bool UInlineCurveLibrary::HasInlineCurveData_Color(const FInlineColorCurve& Target)
{
	return Target.HasAnyData();
}

UCurveLinearColor* UInlineCurveLibrary::GetInlineCurveAsset_Color(const FInlineColorCurve& Target)
{
	return Target.GetCurveAsset();
}

FLinearColor UInlineCurveLibrary::GetInlineCurveValue_Color(const FInlineColorCurve& Target, const float InTime)
{
	return Target.GetValue(InTime);
}

void UInlineCurveLibrary::GetInlineCurveTimeRange_Color(const FInlineColorCurve& Target, float& Min, float& Max)
{
	return Target.GetTimeRange(Min, Max);
}

void UInlineCurveLibrary::GetInlineCurveValueRange_Color(const FInlineColorCurve& Target, FLinearColor& Min, FLinearColor& Max)
{
	return Target.GetValueRange(Min, Max);
}
