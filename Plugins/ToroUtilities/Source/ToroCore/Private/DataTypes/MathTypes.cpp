// Copyright (C) RedCraft86. All Rights Reserved.

#include "DataTypes/MathTypes.h"

FRichCurve* FInlineFloatCurve::GetRichCurve()
{
	return Curve.GetRichCurve();
}

const FRichCurve* FInlineFloatCurve::GetRichCurveConst() const
{
	return Curve.GetRichCurveConst();
}

bool FInlineFloatCurve::HasAnyData() const
{
	return GetRichCurveConst()->HasAnyData();
}

UCurveFloat* FInlineFloatCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

void FInlineFloatCurve::RemovePoint(const float Time)
{
	const FKeyHandle Handle = GetRichCurve()->FindKey(Time);
	if (Handle.IsValid()) GetRichCurve()->DeleteKey(Handle);
}

void FInlineFloatCurve::AddOrUpdatePoint(const float Time, const float Value, const ERichCurveTangentMode TangentMode)
{
	GetRichCurve()->SetKeyTangentMode(GetRichCurve()->UpdateOrAddKey(Time, Value), TangentMode);
}

float FInlineFloatCurve::GetValue(const float InTime) const
{
	return GetRichCurveConst()->Eval(InTime);
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

FRichCurve* FInlineVectorCurve::GetRichCurve(const int32 Index)
{
	return Curve.GetRichCurve(Index);
}

const FRichCurve* FInlineVectorCurve::GetRichCurveConst(const int32 Index) const
{
	return Curve.GetRichCurveConst(Index);
}

bool FInlineVectorCurve::HasAnyData() const
{
	return GetRichCurveConst(0)->HasAnyData()
		|| GetRichCurveConst(1)->HasAnyData()
		|| GetRichCurveConst(2)->HasAnyData();
}

UCurveVector* FInlineVectorCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

void FInlineVectorCurve::RemovePoints(const float Time)
{
	for (int i = 0; i < 3; i++)
	{
		const FKeyHandle Handle = GetRichCurve(i)->FindKey(Time);
		if (Handle.IsValid()) GetRichCurve(i)->DeleteKey(Handle);
	}
}

void FInlineVectorCurve::AddOrUpdatePoints(const float Time, const FVector& Value, const ERichCurveTangentMode TangentMode)
{
	for (int i = 0; i < 3; i++)
	{
		GetRichCurve(i)->SetKeyTangentMode(GetRichCurve(i)->UpdateOrAddKey(
			Time, Value.Component(i)), TangentMode);
	}
}

FVector FInlineVectorCurve::GetValue(const float InTime) const
{
	return {
		GetRichCurveConst(0)->Eval(InTime),
		GetRichCurveConst(1)->Eval(InTime),
		GetRichCurveConst(2)->Eval(InTime)
	};
}

void FInlineVectorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3;
	float Max1, Max2, Max3;

	GetRichCurveConst(0)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(1)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(2)->GetTimeRange(Min3, Max3);

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
	float MinX, MinY, MinZ;
	float MaxX, MaxY, MaxZ;

	GetRichCurveConst(0)->GetValueRange(MinX, MaxX);
	GetRichCurveConst(1)->GetValueRange(MinY, MaxY);
	GetRichCurveConst(2)->GetValueRange(MinZ, MaxZ);

	Min = { MinX, MinY, MinZ };
	Max = { MaxX, MaxY, MaxZ };
}

FRichCurve* FInlineColorCurve::GetRichCurve(const int32 Index)
{
	if (Index < 0 || Index >= 4)
	{
		return nullptr;
	}

	if (IsValid(Curve.ExternalCurve))
	{
		return &Curve.ExternalCurve->FloatCurves[Index];
	}
	
	return &Curve.ColorCurves[Index];
}

const FRichCurve* FInlineColorCurve::GetRichCurveConst(const int32 Index) const
{
	if (Index < 0 || Index >= 4)
	{
		return nullptr;
	}

	if (IsValid(Curve.ExternalCurve))
	{
		return &Curve.ExternalCurve->FloatCurves[Index];
	}
	
	return &Curve.ColorCurves[Index];
}

bool FInlineColorCurve::HasAnyData() const
{
	return GetRichCurveConst(0)->HasAnyData()
		|| GetRichCurveConst(1)->HasAnyData()
		|| GetRichCurveConst(2)->HasAnyData()
		|| GetRichCurveConst(3)->HasAnyData();
}

UCurveLinearColor* FInlineColorCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

void FInlineColorCurve::RemovePoints(const float Time)
{
	for (int i = 0; i < 4; i++)
	{
		const FKeyHandle Handle = GetRichCurve(i)->FindKey(Time);
		if (Handle.IsValid()) GetRichCurve(i)->DeleteKey(Handle);
	}
}

void FInlineColorCurve::AddOrUpdatePoints(const float Time, const FLinearColor& Value, const ERichCurveTangentMode TangentMode)
{
	for (int i = 0; i < 4; i++)
	{
		GetRichCurve(i)->SetKeyTangentMode(GetRichCurve(i)->UpdateOrAddKey(
			Time, Value.Component(i)), TangentMode);
	}
}

FLinearColor FInlineColorCurve::GetValue(const float InTime) const
{
	FLinearColor Result = {
		FMath::Max(0.0f, GetRichCurveConst(0)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(1)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(2)->Eval(InTime))
	};
	
	const FRichCurve* Alpha = GetRichCurveConst(3);
	Result.A = Alpha->HasAnyData() ? FMath::Max(0.0f, Alpha->Eval(InTime)) : 1.0f;

	return Result;
}

void FInlineColorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3, Min4;
	float Max1, Max2, Max3, Max4;

	GetRichCurveConst(0)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(1)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(2)->GetTimeRange(Min3, Max3);
	GetRichCurveConst(3)->GetTimeRange(Min4, Max4);

	Min = FMath::Min3(Min1, Min2, FMath::Min(Min3, Min4));
	Max = FMath::Max3(Max1, Max2, FMath::Min(Max3, Max4));
}

void FInlineColorCurve::GetTimeRange(double& Min, double& Max) const
{
	float X, Y;
	GetTimeRange(X, Y);
	Min = X; Max = Y;
}

void FInlineColorCurve::GetValueRange(FLinearColor& Min, FLinearColor& Max) const
{
	float MinR, MinG, MinB, MinA = 1.0f;
	float MaxR, MaxG, MaxB, MaxA = 1.0f;

	GetRichCurveConst(0)->GetValueRange(MinR, MaxR);
	GetRichCurveConst(1)->GetValueRange(MinG, MaxG);
	GetRichCurveConst(2)->GetValueRange(MinB, MaxB);

	const FRichCurve* Alpha = GetRichCurveConst(3);
	if (Alpha->HasAnyData()) Alpha->GetTimeRange(MinA, MaxA);

	Min = { MinR, MinG, MinB, MinA };
	Max = { MaxR, MaxG, MaxB, MaxA };
}
