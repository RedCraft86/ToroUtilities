// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroMathLibrary.generated.h"

/**
 * Exposes math constants, color generation, noise, and array helpers.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroMathLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Gets UE_SMALL_NUMBER.
	 * @return 1.e-8 <c>~0.00000001</c>
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Small"))
		static float SmallNumber();

	/**
	 * Gets UE_KINDA_SMALL_NUMBER.
	 * @return 1.e-4 <c>~0.0001</c>
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Kinda Small"))
		static float KindaSmallNumber();

	/**
	 * Gets UE_BIG_NUMBER.
	 * @return 3.4e+38 <c>~FLT_MAX</c>
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Big"))
		static float BigNumber();

	/**
	 * Measures the distance between two positions in the XY plane.
	 * @param A First position.
	 * @param B Second position.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Vector")
		static double GetHorizontalDistance(const FVector A, const FVector B);

	/**
	 * Converts a color temperature in kelvin to a linear color.
	 * @param Temperature Color temperature in kelvin.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor TemperatureToLinearColor(const float Temperature);

	/**
	 * Generates a random linear color.
	 * @param bTrueRandom Whether to generate each RGB channel independently.
	 * @param bRandomAlpha Whether to randomize alpha instead of using full opacity.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor RandomLinearColor(const bool bTrueRandom, const bool bRandomAlpha = false);

	/**
	 * Converts a color temperature in kelvin to an 8-bit color.
	 * @param Temperature Color temperature in kelvin.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor TemperatureToColor(const float Temperature);

	/**
	 * Generates a random 8-bit color.
	 * @param bTrueRandom Whether to generate each RGB channel independently.
	 * @param bRandomAlpha Whether to randomize alpha instead of using full opacity.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor RandomColor(const bool bTrueRandom, const bool bRandomAlpha =false);

	/**
	 * Samples two-dimensional Perlin noise.
	 * @param Position Sample coordinates.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Random")
		static float PerlinNoise2D(FVector2D Position);

	/**
	 * Samples three-dimensional Perlin noise.
	 * @param Position Sample coordinates.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Random")
		static float PerlinNoise3D(const FVector& Position);

	/**
	 * Folds an array from a starting value using a binary operation.
	 * @tparam T Array element and result type.
	 * @param InArray Elements to process in order.
	 * @param BaseValue Initial accumulator value.
	 * @param Func Operation applied to the accumulator and each element.
	 */
	template<typename T>
	static T ArrayOperate(const TArray<T>& InArray, const T& BaseValue, const TFunction<T(const T&, const T&)>& Func)
	{
		T Result = BaseValue;
		for (int32 i = 0; i < InArray.Num(); i++)
		{
			Result = Func(Result, InArray[i]);
		}
		return Result;
	}

	/**
	 * Adds all array elements; an empty array produces zero.
	 * @tparam T Element type supporting addition and zero construction.
	 * @param InArray Elements to add.
	 * @param BaseValue Initial accumulator value.
	 */
	template<typename T>
	static T ArraySum(const TArray<T>& InArray, const T& BaseValue = T(0))
	{
		return ArrayOperate<T>(InArray, BaseValue, [](const T& A, const T& B)
		{
			return A + B;
		});
	}

	/**
	 * Multiplies all array elements; an empty array produces one.
	 * @tparam T Element type supporting multiplication and one construction.
	 * @param InArray Elements to multiply.
	 * @param BaseValue Initial accumulator value.
	 */
	template<typename T>
	static T ArrayProduct(const TArray<T>& InArray, const T& BaseValue = T(1))
	{
		return ArrayOperate<T>(InArray, BaseValue, [](const T& A, const T& B)
		{
			return A * B;
		});
	}
};
