// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroMathLibrary.generated.h"

/**
 * A utility library providing functions for Mathematical operations.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroMathLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Returns a very small number.
	 * @return 1.e-8 <c>~0.00000001</c>
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Small"))
		static float SmallNumber() { return UE_SMALL_NUMBER; }

	/**
	 * Returns a kinda small number.
	 * @return 1.e-4 <c>~0.0001</c>
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Kinda Small"))
		static float KindaSmallNumber() { return UE_KINDA_SMALL_NUMBER; }

	/**
	 * Returns a big number.
	 * @return 3.4e+38 <c>~FLT_MAX</c>
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Big"))
		static float BigNumber() { return UE_BIG_NUMBER; }

	/**
	 * Calculates the Euclidean distance between two points on the XY plane.
	 * This ignores the Z (height) component entirely.
	 * @param A Starting vector position.
	 * @param B Ending vector position.
	 * @return The horizontal distance between A and B.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Vector")
		static double GetHorizontalDistance(const FVector A, const FVector B);

	/**
	 * Composes two rotators together.
	 * This is exposed as an operator (+) node in Blueprints.
	 * @param A The base rotator.
	 * @param B The rotation to add.
	 * @return The combined FRotator.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (DisplayName = "rotator + rotator",
		CompactNodeTitle = "+", ScriptMethod = "Add", ScriptOperator = "+;+=",
		Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = true))
		static FRotator Add_RotatorRotator(const FRotator A, const FRotator B);

	/**
	 * Converts a blackbody temperature (in Kelvin) into a Linear Color.
	 * @param Temperature The temperature in Kelvin (standard range is 1000 to 15000).
	 * @return The resulting Linear Color.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor TemperatureToLinearColor(const float Temperature);

	/**
	 * Generates a random but pretty Linear Color.
	 * @param bTrueRandom If true, generate a truly random color that may or may not be pretty.
	 * @param bRandomAlpha If true, the Alpha (opacity) channel is also randomized.
	 * @return A randomized Linear color.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor RandomLinearColor(const bool bTrueRandom, const bool bRandomAlpha = false);

	/**
	 * Converts a blackbody temperature (in Kelvin) into a standard 8-bit Color.
	 * @param Temperature The temperature in Kelvin.
	 * @return The resulting 8-bit color.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor TemperatureToColor(const float Temperature);

	/**
	 * Generates a random but pretty standard 8-bit Color.
	 * @param bTrueRandom If true, generate a truly random color that may or may not be pretty.
	 * @param bRandomAlpha If true, the Alpha channel is randomized.
	 * @return A randomized 8-bit color.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor RandomColor(const bool bTrueRandom, const bool bRandomAlpha =false);

	/**
	* Returns a perlin noise value between -1 and 1 at the given position. (Uses <c>FMath::PerlinNoise2D</c>)
	* @param Position - The position to get the noise value for.
	* @returns The noise value at the given position.
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Random")
		static float PerlinNoise2D(FVector2D Position);

	/**
	* Returns a perlin noise value between -1 and 1 at the given position. (Uses <c>FMath::PerlinNoise3D</c>)
	* @param Position - The position to get the noise value for.
	* @returns The noise value at the given position.
	*/
	UFUNCTION(BlueprintPure, Category = "Math|Random")
		static float PerlinNoise3D(const FVector& Position);

	/**
	 * Applies an operation to all values inside the given array, resulting in a final value.
	 * @tparam T Numeric type including structs such as Vectors.
	 * @param InArray Array of T values to operate.
	 * @param BaseValue Base value that will be operated on.
	 * @param Func Function that will handle the operation.
	 * @return Result of the operation
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
	 * Adds all values inside the given array.
	 * @tparam T Numeric type including structs such as Vectors.
	 * @param InArray Array of T values to sum up.
	 * @return Sum of all values in the array
	 */
	template<typename T>
	static T ArraySum(const TArray<T>& InArray)
	{
		return ArrayOperate<T>(InArray, T(0), [](const T& A, const T& B)
		{
			return A + B;
		});
	}

	/**
	 * Multiplies all values inside the given array.
	 * @tparam T Numeric type including structs such as Vectors.
	 * @param InArray Array of T values to sum up.
	 * @return Sum of all values in the array
	 */
	template<typename T>
	static T ArrayProduct(const TArray<T>& InArray)
	{
		return ArrayOperate<T>(InArray, T(1), [](const T& A, const T& B)
		{
			return A * B;
		});
	}

	/**
	 * Adds all values inside the given array and divides by the number of entries.
	 * @tparam T Numeric type including structs such as Vectors.
	 * @param InArray Array of T values to sum up.
	 * @return Average of all values in the array.
	 */
	template<typename T>
	static T ArrayAverage(const TArray<T>& InArray)
	{
		return InArray.IsEmpty() ? T(0) : ArraySum<T>(InArray) / InArray.Num();
	}
};
