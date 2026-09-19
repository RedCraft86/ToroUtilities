// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InlineCurves.generated.h"

/**
 * Wraps a runtime float curve for inline editing and convenient evaluation.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FInlineFloatCurve final
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = InlineCurve)
		FRuntimeFloatCurve Curve;

	FORCEINLINE operator FRuntimeFloatCurve&() { return Curve; }
	FORCEINLINE operator const FRuntimeFloatCurve&() const { return Curve; }

	FORCEINLINE UCurveFloat* GetCurveAsset() const { return Curve.ExternalCurve; }
	FORCEINLINE bool HasAnyData() const { return GetRichCurve()->HasAnyData(); }

	FRichCurve* GetRichCurve();
	const FRichCurve* GetRichCurve() const;

	void RemovePoint(const float Time);
	void AddOrUpdatePoint(const float Time, const float Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	float GetValue(const float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetValueRange(float& Min, float& Max) const;
};

/**
 * Wraps a runtime vector curve for inline editing and convenient evaluation.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FInlineVectorCurve final
{
	GENERATED_BODY()

	enum Components : uint8
	{
		X = 0,
		Y = 1,
		Z = 2,
		NUM = 3
	};

	UPROPERTY(EditAnywhere, Category = InlineCurve)
		FRuntimeVectorCurve Curve;

	FORCEINLINE operator FRuntimeVectorCurve&() { return Curve; }
	FORCEINLINE operator const FRuntimeVectorCurve&() const { return Curve; }

	FORCEINLINE UCurveVector* GetCurveAsset() const { return Curve.ExternalCurve; }
	FORCEINLINE bool HasAnyData() const
	{
		return GetRichCurve(Components::X)->HasAnyData()
			|| GetRichCurve(Components::Y)->HasAnyData()
			|| GetRichCurve(Components::Z)->HasAnyData();
	}

	FRichCurve* GetRichCurve(int32 Index);
	const FRichCurve* GetRichCurve(int32 Index) const;

	void RemovePoints(const float Time);
	void AddOrUpdatePoints(const float Time, const FVector& Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	FVector GetValue(const float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetValueRange(FVector& Min, FVector& Max) const;
};

/**
 * Wraps a runtime linear-color curve for inline editing and evaluation.
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FInlineColorCurve final
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

	UPROPERTY(EditAnywhere, Category = InlineCurve)
		FRuntimeCurveLinearColor Curve;

	FORCEINLINE operator FRuntimeCurveLinearColor&() { return Curve; }
	FORCEINLINE operator const FRuntimeCurveLinearColor&() const { return Curve; }

	FORCEINLINE UCurveLinearColor* GetCurveAsset() const { return Curve.ExternalCurve; }
	FORCEINLINE bool HasAnyData() const
	{
		return GetRichCurve(Components::R)->HasAnyData()
			|| GetRichCurve(Components::G)->HasAnyData()
			|| GetRichCurve(Components::B)->HasAnyData()
			|| GetRichCurve(Components::A)->HasAnyData();
	}

	FRichCurve* GetRichCurve(int32 Index);
	const FRichCurve* GetRichCurve(int32 Index) const;

	void RemovePoints(const float Time);
	void AddOrUpdatePoints(const float Time, const FLinearColor& Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	FLinearColor GetValue(const float InTime) const;
	void GetTimeRange(float& Min, float& Max) const;
	void GetValueRange(FLinearColor& Min, FLinearColor& Max) const;
};

/**
 * Exposes inline float, vector, and linear-color curve queries to Blueprints.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UInlineCurveLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Float)")
	static bool HasInlineCurveData_Float(const FInlineFloatCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Float)")
	static UCurveFloat* GetInlineCurveAsset_Float(const FInlineFloatCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Float)")
	static float GetInlineCurveValue_Float(const FInlineFloatCurve& Target, const float InTime);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Float)")
	static void GetInlineCurveTimeRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Float)")
	static void GetInlineCurveValueRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Vector)")
	static bool HasInlineCurveData_Vector(const FInlineVectorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Vector)")
	static UCurveVector* GetInlineCurveAsset_Vector(const FInlineVectorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Vector)")
	static FVector GetInlineCurveValue_Vector(const FInlineVectorCurve& Target, const float InTime);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Vector)")
	static void GetInlineCurveTimeRange_Vector(const FInlineVectorCurve& Target, float& Min, float& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Vector)")
	static void GetInlineCurveValueRange_Vector(const FInlineVectorCurve& Target, FVector& Min, FVector& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Color)")
	static bool HasInlineCurveData_Color(const FInlineColorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Color)")
	static UCurveLinearColor* GetInlineCurveAsset_Color(const FInlineColorCurve& Target);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Color)")
	static FLinearColor GetInlineCurveValue_Color(const FInlineColorCurve& Target, const float InTime);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Color)")
	static void GetInlineCurveTimeRange_Color(const FInlineColorCurve& Target, float& Min, float& Max);

	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Color)")
	static void GetInlineCurveValueRange_Color(const FInlineColorCurve& Target, FLinearColor& Min, FLinearColor& Max);
};