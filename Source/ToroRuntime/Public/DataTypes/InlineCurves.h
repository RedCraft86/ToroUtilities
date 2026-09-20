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

	/**
	 * Tthe active rich curve for modification.
	 */
	FRichCurve* GetRichCurve();

	/**
	 * The active rich curve for read-only access.
	 */
	const FRichCurve* GetRichCurve() const;

	/**
	 * @return the assigned external curve asset, or nullptr if none is assigned.
	 */
	UCurveFloat* GetCurveAsset() const;

	/**
	 * @return whether the active curve has any keys.
	 */
	bool HasAnyData() const;

	/**
	 * Removes the key at the specified time, if present.
	 * @param Time Time of the key to change.
	 */
	void RemovePoint(const float Time);

	/**
	 * Adds or updates a key at the specified time using the requested tangent mode.
	 * @param Time Time of the key to change.
	 * @param Value Value to assign at the specified time.
	 * @param Tangent Tangent mode for the new or updated key.
	 */
	void AddOrUpdatePoint(const float Time, const float Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	/**
	 * Evaluates the curve at InTime.
	 * @param InTime Time at which to evaluate the curve.
	 * @return value obtained by evaluating the curve at InTime.
	 */
	float GetValue(const float InTime) const;

	/**
	 * Returns the minimum and maximum key times.
	 * @param Min Receives the minimum key time.
	 * @param Max Receives the maximum key time.
	 */
	void GetTimeRange(float& Min, float& Max) const;

	/**
	 * Returns the minimum and maximum curve values.
	 * @param Min Receives the minimum curve value.
	 * @param Max Receives the maximum curve value.
	 */
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

	/**
	 * Returns the selected active component curve for modification.
	 * @param Index Index of the component curve.
	 * @return the selected active component curve for modification.
	 */
	FRichCurve* GetRichCurve(int32 Index);

	/**
	 * Returns the selected active component curve for read-only access.
	 * @param Index Index of the component curve.
	 * @return the selected active component curve for read-only access.
	 */
	const FRichCurve* GetRichCurve(int32 Index) const;

	/**
	 * @return the assigned external curve asset, or nullptr if none is assigned.
	 */
	UCurveVector* GetCurveAsset() const;

	/**
	 * @return whether any component curve contains keys.
	 */
	bool HasAnyData() const;

	/**
	 * Removes keys at Time from each component curve.
	 * @param Time Time of the key to change.
	 */
	void RemovePoints(const float Time);

	/**
	 * Adds or updates X, Y, and Z keys at Time.
	 * @param Time Time of the key to change.
	 * @param Value Value to assign at the specified time.
	 * @param Tangent Tangent mode for the new or updated key.
	 */
	void AddOrUpdatePoints(const float Time, const FVector& Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	/**
	 * Evaluates the component curves at InTime.
	 * @param InTime Time at which to evaluate the curve.
	 * @return value obtained by evaluating the component curves at InTime.
	 */
	FVector GetValue(const float InTime) const;

	/**
	 * Returns the overall key time range of the component curves.
	 * @param Min Receives the minimum key time.
	 * @param Max Receives the maximum key time.
	 */
	void GetTimeRange(float& Min, float& Max) const;

	/**
	 * Returns each component curve's value range.
	 * @param Min Receives the minimum curve value.
	 * @param Max Receives the maximum curve value.
	 */
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

	/**
	 * Returns the selected active color channel for modification.
	 * @param Index Index of the component curve.
	 * @return the selected active color channel for modification.
	 */
	FRichCurve* GetRichCurve(int32 Index);

	/**
	 * Returns the selected active color channel for read-only access.
	 * @param Index Index of the component curve.
	 * @return the selected active color channel for read-only access.
	 */
	const FRichCurve* GetRichCurve(int32 Index) const;

	/**
	 * @return the assigned external curve asset, or nullptr if none is assigned.
	 */
	UCurveLinearColor* GetCurveAsset() const;

	/**
	 * @return whether any color channel contains keys.
	 */
	bool HasAnyData() const;

	/**
	 * Removes keys at Time from every color channel.
	 * @param Time Time of the key to change.
	 */
	void RemovePoints(const float Time);

	/**
	 * Adds or updates RGBA keys at Time.
	 * @param Time Time of the key to change.
	 * @param Value Value to assign at the specified time.
	 * @param Tangent Tangent mode for the new or updated key.
	 */
	void AddOrUpdatePoints(const float Time, const FLinearColor& Value, const ERichCurveTangentMode Tangent = RCTM_Auto);

	/**
	 * Evaluates the color channels at InTime.
	 * @param InTime Time at which to evaluate the curve.
	 * @return value obtained by evaluating the color channels at InTime.
	 */
	FLinearColor GetValue(const float InTime) const;

	/**
	 * Returns the overall key time range of the color channels.
	 * @param Min Receives the minimum key time.
	 * @param Max Receives the maximum key time.
	 */
	void GetTimeRange(float& Min, float& Max) const;

	/**
	 * Returns each color channel's value range.
	 * @param Min Receives the minimum curve value.
	 * @param Max Receives the maximum curve value.
	 */
	void GetValueRange(FLinearColor& Min, FLinearColor& Max) const;
};

/**
 * Exposes inline curve queries to Blueprints.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UInlineCurveLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Returns whether the inline curve contains any keys.
	 * @param Target Curve to inspect.
	 * @return whether the inline curve contains any keys.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Float)")
		static bool HasInlineCurveData_Float(const FInlineFloatCurve& Target);

	/**
	 * Returns the assigned external curve asset.
	 * @param Target Curve to inspect.
	 * @return the assigned external curve asset.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Float)")
		static UCurveFloat* GetInlineCurveAsset_Float(const FInlineFloatCurve& Target);

	/**
	 * Evaluates the inline curve at InTime.
	 * @param Target Curve to inspect.
	 * @param InTime Time at which to evaluate the curve.
	 * @return value obtained by evaluating the inline curve at InTime.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Float)")
		static float GetInlineCurveValue_Float(const FInlineFloatCurve& Target, const float InTime);

	/**
	 * Returns the inline curve time range through Min and Max.
	 * @param Target Curve to inspect.
	 * @param Min Receives the minimum key time.
	 * @param Max Receives the maximum key time.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Float)")
		static void GetInlineCurveTimeRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max);

	/**
	 * Returns the inline curve value range through Min and Max.
	 * @param Target Curve to inspect.
	 * @param Min Receives the minimum curve value.
	 * @param Max Receives the maximum curve value.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Float)")
		static void GetInlineCurveValueRange_Float(const FInlineFloatCurve& Target, float& Min, float& Max);

	/**
	 * Returns whether the inline curve contains any keys.
	 * @param Target Curve to inspect.
	 * @return whether the inline curve contains any keys.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Vector)")
		static bool HasInlineCurveData_Vector(const FInlineVectorCurve& Target);

	/**
	 * Returns the assigned external curve asset.
	 * @param Target Curve to inspect.
	 * @return the assigned external curve asset.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Vector)")
		static UCurveVector* GetInlineCurveAsset_Vector(const FInlineVectorCurve& Target);

	/**
	 * Evaluates the inline curve at InTime.
	 * @param Target Curve to inspect.
	 * @param InTime Time at which to evaluate the curve.
	 * @return value obtained by evaluating the inline curve at InTime.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Vector)")
		static FVector GetInlineCurveValue_Vector(const FInlineVectorCurve& Target, const float InTime);

	/**
	 * Returns the inline curve time range through Min and Max.
	 * @param Target Curve to inspect.
	 * @param Min Receives the minimum key time.
	 * @param Max Receives the maximum key time.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Vector)")
		static void GetInlineCurveTimeRange_Vector(const FInlineVectorCurve& Target, float& Min, float& Max);

	/**
	 * Returns the inline curve value range through Min and Max.
	 * @param Target Curve to inspect.
	 * @param Min Receives the minimum curve value.
	 * @param Max Receives the maximum curve value.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Vector)")
		static void GetInlineCurveValueRange_Vector(const FInlineVectorCurve& Target, FVector& Min, FVector& Max);

	/**
	 * Returns whether the inline curve contains any keys.
	 * @param Target Curve to inspect.
	 * @return whether the inline curve contains any keys.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Has Any Data (Color)")
		static bool HasInlineCurveData_Color(const FInlineColorCurve& Target);

	/**
	 * Returns the assigned external curve asset.
	 * @param Target Curve to inspect.
	 * @return the assigned external curve asset.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Curve Asset (Color)")
		static UCurveLinearColor* GetInlineCurveAsset_Color(const FInlineColorCurve& Target);

	/**
	 * Evaluates the inline curve at InTime.
	 * @param Target Curve to inspect.
	 * @param InTime Time at which to evaluate the curve.
	 * @return value obtained by evaluating the inline curve at InTime.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value (Color)")
		static FLinearColor GetInlineCurveValue_Color(const FInlineColorCurve& Target, const float InTime);

	/**
	 * Returns the inline curve time range through Min and Max.
	 * @param Target Curve to inspect.
	 * @param Min Receives the minimum key time.
	 * @param Max Receives the maximum key time.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Time Range (Color)")
		static void GetInlineCurveTimeRange_Color(const FInlineColorCurve& Target, float& Min, float& Max);

	/**
	 * Returns the inline curve value range through Min and Max.
	 * @param Target Curve to inspect.
	 * @param Min Receives the minimum curve value.
	 * @param Max Receives the maximum curve value.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Curves|Inline", DisplayName = "Get Value Range (Color)")
		static void GetInlineCurveValueRange_Color(const FInlineColorCurve& Target, FLinearColor& Min, FLinearColor& Max);
};