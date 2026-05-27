// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"
#include "InlineCurves.generated.h"

USTRUCT(BlueprintType)
struct TOROCORE_API FInlineFloatCurve final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = InlineFloatCurve)
		FRuntimeFloatCurve Curve;

	FORCEINLINE UCurveFloat* GetCurveAsset() const { return Curve.ExternalCurve; }
	FORCEINLINE bool HasAnyData() const
	{
		return GetRichCurveConst()->HasAnyData();
	}

	FORCEINLINE FRichCurve* GetRichCurve() { return Curve.GetRichCurve(); }
	FORCEINLINE const FRichCurve* GetRichCurveConst() const { return Curve.GetRichCurveConst(); }

	void RemovePoint(const float Time);
	void AddOrUpdatePoint(const float Time, const float Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	float GetValue(float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetTimeRange(double& Min, double& Max) const;
	void GetValueRange(float& Min, float& Max) const;
	void GetValueRange(double& Min, double& Max) const;
};

USTRUCT(BlueprintType)
struct TOROCORE_API FInlineVectorCurve final
{
	GENERATED_BODY()

	enum Components : uint8
	{
		X = 0,
		Y = 1,
		Z = 2,
		NUM = 3
	};

	UPROPERTY(EditAnywhere, Category = InlineVectorCurve)
		FRuntimeVectorCurve Curve;

	FORCEINLINE UCurveVector* GetCurveAsset() const { return Curve.ExternalCurve; }
	FORCEINLINE bool HasAnyData() const
	{
		return GetRichCurveConst(Components::X)->HasAnyData()
			|| GetRichCurveConst(Components::Y)->HasAnyData()
			|| GetRichCurveConst(Components::Z)->HasAnyData();
	}

	FORCEINLINE FRichCurve* GetRichCurve(int32 Index) { return Curve.GetRichCurve(Index); }
	FORCEINLINE const FRichCurve* GetRichCurveConst(int32 Index) const { return Curve.GetRichCurveConst(Index); }

	void RemovePoints(const float Time);
	void AddOrUpdatePoints(const float Time, const FVector& Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	FVector GetValue(float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetTimeRange(double& Min, double& Max) const;
	void GetValueRange(FVector& Min, FVector& Max) const;
};

USTRUCT(BlueprintType)
struct TOROCORE_API FInlineColorCurve final
{
	GENERATED_BODY()

	enum Components : uint8
	{
		R = 0,
		G = 1,
		B = 2,
		A = 3,
		NUM = 4
	};

	UPROPERTY(EditAnywhere, Category = InlineColorCurve)
		FRuntimeCurveLinearColor Curve;

	FORCEINLINE UCurveLinearColor* GetCurveAsset() const { return Curve.ExternalCurve; }
	FORCEINLINE bool HasAnyData() const
	{
		return GetRichCurveConst(Components::R)->HasAnyData()
			|| GetRichCurveConst(Components::G)->HasAnyData()
			|| GetRichCurveConst(Components::B)->HasAnyData()
			|| GetRichCurveConst(Components::A)->HasAnyData();
	}

	FRichCurve* GetRichCurve(int32 Index);
	const FRichCurve* GetRichCurveConst(int32 Index) const;

	void RemovePoints(const float Time);
	void AddOrUpdatePoints(const float Time, const FLinearColor& Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	FLinearColor GetValue(float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetTimeRange(double& Min, double& Max) const;
	void GetValueRange(FLinearColor& Min, FLinearColor& Max) const;
};
