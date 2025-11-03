// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "DataTypes/InlineCurves.h"

namespace CurveComponents
{
	inline uint8 X = 0;
	inline uint8 Y = 1;
	inline uint8 Z = 2;
	inline uint8 Vector_MAX = Z;

	inline uint8 R = 0;
	inline uint8 G = 1;
	inline uint8 B = 2;
	inline uint8 A = 3;
	inline uint8 Color_MAX = A;
}

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
	return GetRichCurveConst(CurveComponents::X)->HasAnyData()
		|| GetRichCurveConst(CurveComponents::Y)->HasAnyData()
		|| GetRichCurveConst(CurveComponents::Z)->HasAnyData();
}

UCurveVector* FInlineVectorCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

void FInlineVectorCurve::RemovePoints(const float Time)
{
	for (int i = 0; i <= CurveComponents::Vector_MAX; i++)
	{
		const FKeyHandle Handle = GetRichCurve(i)->FindKey(Time);
		if (Handle.IsValid()) GetRichCurve(i)->DeleteKey(Handle);
	}
}

void FInlineVectorCurve::AddOrUpdatePoints(const float Time, const FVector& Value, const ERichCurveTangentMode TangentMode)
{
	for (int i = 0; i <= CurveComponents::Vector_MAX; i++)
	{
		GetRichCurve(i)->SetKeyTangentMode(GetRichCurve(i)->UpdateOrAddKey(
			Time, Value.Component(i)), TangentMode);
	}
}

FVector FInlineVectorCurve::GetValue(const float InTime) const
{
	return {
		GetRichCurveConst(CurveComponents::X)->Eval(InTime),
		GetRichCurveConst(CurveComponents::Y)->Eval(InTime),
		GetRichCurveConst(CurveComponents::Z)->Eval(InTime)
	};
}

void FInlineVectorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3;
	float Max1, Max2, Max3;

	GetRichCurveConst(CurveComponents::X)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(CurveComponents::Y)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(CurveComponents::Z)->GetTimeRange(Min3, Max3);

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

	GetRichCurveConst(CurveComponents::X)->GetValueRange(MinX, MaxX);
	GetRichCurveConst(CurveComponents::Y)->GetValueRange(MinY, MaxY);
	GetRichCurveConst(CurveComponents::Z)->GetValueRange(MinZ, MaxZ);

	Min = { MinX, MinY, MinZ };
	Max = { MaxX, MaxY, MaxZ };
}

FRichCurve* FInlineColorCurve::GetRichCurve(const int32 Index)
{
	if (Index < 0 || Index > CurveComponents::Color_MAX)
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
	if (Index < 0 || Index > CurveComponents::Color_MAX)
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
	return GetRichCurveConst(CurveComponents::R)->HasAnyData()
		|| GetRichCurveConst(CurveComponents::G)->HasAnyData()
		|| GetRichCurveConst(CurveComponents::B)->HasAnyData()
		|| GetRichCurveConst(CurveComponents::A)->HasAnyData();
}

UCurveLinearColor* FInlineColorCurve::GetCurveAsset() const
{
	return Curve.ExternalCurve;
}

void FInlineColorCurve::RemovePoints(const float Time)
{
	for (int i = 0; i <= CurveComponents::Color_MAX; i++)
	{
		const FKeyHandle Handle = GetRichCurve(i)->FindKey(Time);
		if (Handle.IsValid()) GetRichCurve(i)->DeleteKey(Handle);
	}
}

void FInlineColorCurve::AddOrUpdatePoints(const float Time, const FLinearColor& Value, const ERichCurveTangentMode TangentMode)
{
	for (int i = 0; i <= CurveComponents::Color_MAX; i++)
	{
		GetRichCurve(i)->SetKeyTangentMode(GetRichCurve(i)->UpdateOrAddKey(
			Time, Value.Component(i)), TangentMode);
	}
}

FLinearColor FInlineColorCurve::GetValue(const float InTime) const
{
	FLinearColor Result = {
		FMath::Max(0.0f, GetRichCurveConst(CurveComponents::R)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(CurveComponents::G)->Eval(InTime)),
		FMath::Max(0.0f, GetRichCurveConst(CurveComponents::B)->Eval(InTime))
	};
	
	const FRichCurve* Alpha = GetRichCurveConst(CurveComponents::A);
	Result.A = Alpha->HasAnyData() ? FMath::Max(0.0f, Alpha->Eval(InTime)) : 1.0f;

	return Result;
}

void FInlineColorCurve::GetTimeRange(float& Min, float& Max) const
{
	float Min1, Min2, Min3, Min4;
	float Max1, Max2, Max3, Max4;

	GetRichCurveConst(CurveComponents::R)->GetTimeRange(Min1, Max1);
	GetRichCurveConst(CurveComponents::G)->GetTimeRange(Min2, Max2);
	GetRichCurveConst(CurveComponents::B)->GetTimeRange(Min3, Max3);
	GetRichCurveConst(CurveComponents::A)->GetTimeRange(Min4, Max4);

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

	GetRichCurveConst(CurveComponents::R)->GetValueRange(MinR, MaxR);
	GetRichCurveConst(CurveComponents::G)->GetValueRange(MinG, MaxG);
	GetRichCurveConst(CurveComponents::B)->GetValueRange(MinB, MaxB);

	const FRichCurve* Alpha = GetRichCurveConst(CurveComponents::A);
	if (Alpha->HasAnyData()) Alpha->GetTimeRange(MinA, MaxA);

	Min = { MinR, MinG, MinB, MinA };
	Max = { MaxR, MaxG, MaxB, MaxA };
}