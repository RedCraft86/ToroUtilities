// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "Curves/CurveLinearColor.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InlineCurves.generated.h"

/**
 * Simple wrapper to better expose RuntimeFloatCurve to C++ and Blueprint.
 */
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

/**
 * Simple wrapper to better expose RuntimeVectorCurve to C++ and Blueprint.
 */
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

/**
 * Simple wrapper to better expose RuntimeColorCurve to C++ and Blueprint.
 */
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

/**
 * Blueprint function wrappers for InlineCurve series of structs
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroInlineCurveLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Checks if the Float Curve has any valid data to evaluate (either an external asset or inline keys).
	 * @param Target The inline float curve structure to check.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Float)")
	static bool HasInlineCurveData_Float(const FInlineFloatCurve& Target)
	{
		return Target.HasAnyData();
	}

	/** 
	 * Returns the external UCurveFloat asset assigned to this structure, if one exists.
	 * @param Target The inline float curve structure.
	 * @return The assigned UCurveFloat asset, or nullptr if only inline keys are used.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Float)")
	static UCurveFloat* GetInlineCurveAsset_Float(const FInlineFloatCurve& Target)
	{
		return Target.GetCurveAsset();
	}

	/** 
	 * Evaluates the float curve at a specific point in time.
	 * @param Target The inline float curve structure.
	 * @param InTime The time coordinate at which to evaluate the curve.
	 * @return The interpolated float value at the given time.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Float)")
	static float GetInlineCurveValue_Float(const FInlineFloatCurve& Target, float InTime)
	{
		return Target.GetValue(InTime);
	}

	/** 
	 * Retrieves the minimum and maximum time values defined in the float curve.
	 * @param Target The inline float curve structure.
	 * @param Min The earliest time point in the curve (Output).
	 * @param Max The latest time point in the curve (Output).
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Float)")
	static void GetInlineCurveTimeRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max)
	{
		Target.GetTimeRange(Min, Max);
	}

	/** 
	 * Retrieves the minimum and maximum float values found across the entire curve.
	 * @param Target The inline float curve structure.
	 * @param Min The lowest value found in the curve (Output).
	 * @param Max The highest value found in the curve (Output).
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Float)")
	static void GetInlineCurveValueRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max)
	{
		Target.GetValueRange(Min, Max);
	}

	/** 
	 * Checks if the Vector Curve has any valid data to evaluate.
	 * @param Target The inline vector curve structure.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Vector)")
	static bool HasInlineCurveData_Vector(const FInlineVectorCurve& Target)
	{
		return Target.HasAnyData();
	}

	/** 
	 * Returns the external UCurveVector asset assigned to this structure, if one exists.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Vector)")
	static UCurveVector* GetInlineCurveAsset_Vector(const FInlineVectorCurve& Target)
	{
		return Target.GetCurveAsset();
	}

	/** 
	 * Evaluates the vector curve at a specific point in time.
	 * @param Target The inline vector curve structure.
	 * @param InTime The time coordinate at which to evaluate.
	 * @return The interpolated FVector value at the given time.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Vector)")
	static FVector GetInlineCurveValue_Vector(const FInlineVectorCurve& Target, float InTime)
	{
		return Target.GetValue(InTime);
	}

	/** 
	 * Retrieves the time bounds for the vector curve.
	 * @param Target The inline vector curve structure.
	 * @param Min The earliest time point (Output).
	 * @param Max The latest time point (Output).
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Vector)")
	static void GetInlineCurveTimeRange_Vector(const FInlineVectorCurve& Target, float& Min, float& Max)
	{
		Target.GetTimeRange(Min, Max);
	}

	/** 
	 * Retrieves the component-wise value range for the vector curve.
	 * @param Target The inline vector curve structure.
	 * @param Min The vector containing the minimum values for X, Y, and Z (Output).
	 * @param Max The vector containing the maximum values for X, Y, and Z (Output).
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Vector)")
	static void GetInlineCurveValueRange_Vector(const FInlineVectorCurve& Target, FVector& Min, FVector& Max)
	{
		Target.GetValueRange(Min, Max);
	}

	/** 
	 * Checks if the Color Curve has any valid data to evaluate.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Color)")
	static bool HasInlineCurveData_Color(const FInlineColorCurve& Target)
	{
		return Target.HasAnyData();
	}

	/** 
	 * Returns the external UCurveLinearColor asset assigned to this structure, if one exists.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Color)")
	static UCurveLinearColor* GetInlineCurveAsset_Color(const FInlineColorCurve& Target)
	{
		return Target.GetCurveAsset();
	}

	/** 
	 * Evaluates the color curve at a specific point in time.
	 * @param Target The inline color curve structure.
	 * @param InTime The time coordinate at which to evaluate.
	 * @return The interpolated FLinearColor value at the given time.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Color)")
	static FLinearColor GetInlineCurveValue_Color(const FInlineColorCurve& Target, float InTime)
	{
		return Target.GetValue(InTime);
	}

	/** 
	 * Retrieves the time bounds for the color curve.
	 * @param Target The inline color curve structure.
	 * @param Min The earliest time point (Output).
	 * @param Max The latest time point (Output).
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Color)")
	static void GetInlineCurveTimeRange_Color(const FInlineColorCurve& Target, float& Min, float& Max)
	{
		Target.GetTimeRange(Min, Max);
	}

	/** 
	 * Retrieves the component-wise value range for the color curve (RGBA).
	 * @param Target The inline color curve structure.
	 * @param Min The color containing the minimum R, G, B, and A values (Output).
	 * @param Max The color containing the maximum R, G, B, and A values (Output).
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Color)")
	static void GetInlineCurveValueRange_Color(const FInlineColorCurve& Target, FLinearColor& Min, FLinearColor& Max)
	{
		Target.GetValueRange(Min, Max);
	}
};