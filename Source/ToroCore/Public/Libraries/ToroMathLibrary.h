// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroMathLibrary.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroMathLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** 
	 * Returns a very small value (1.e-8). 
	 * Useful for epsilon checks and avoiding division-by-zero errors.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Small"))
		static float SmallNumber() { return UE_SMALL_NUMBER; }

	/** 
	 * Returns a slightly larger small value (1.e-4). 
	 * Recommended for most geometry and physics "nearly equal" comparisons.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Constant", meta = (CompactNodeTitle = "Kinda Small"))
		static float KindaSmallNumber() { return UE_KINDA_SMALL_NUMBER; }

	/** 
	 * Returns a very large value (3.4e+38). 
	 * Useful for initializing minimum/maximum bounds or representing infinity.
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
	UFUNCTION(BlueprintPure, Category = "Math|Rotator", meta = (DisplayName = "rotator + rotator", CompactNodeTitle = "+", ScriptMethod = "Add", ScriptOperator = "+;+=", Keywords = "+ add plus", CommutativeAssociativeBinaryOperator = "true"))
		static FRotator Add_RotatorRotator(const FRotator A, const FRotator B);

	/** 
	 * Converts a blackbody temperature (in Kelvin) into a Linear Color.
	 * @param Temperature The temperature in Kelvin (standard range is 1000 to 15000).
	 * @return The resulting HDR color representation of that temperature.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor TemperatureToLinearColor(const float Temperature);

	/** 
	 * Generates a random but pretty Linear Color.
	 * @param bTrueRandom If true, generate a truly random color that may or may not be pretty.
	 * @param bRandomAlpha If true, the Alpha (opacity) channel is also randomized.
	 * @return A randomized HDR color.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|LinearColor")
		static FLinearColor RandomLinearColor(const bool bTrueRandom, const bool bRandomAlpha);

	/** 
	 * Converts a blackbody temperature (in Kelvin) into a standard 8-bit Color.
	 * @param Temperature The temperature in Kelvin.
	 * @return The resulting LDR (standard) color.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor TemperatureToColor(const float Temperature);

	/** 
	 * Generates a random but pretty standard 8-bit Color.
	 * @param bTrueRandom If true, generate a truly random color that may or may not be pretty.
	 * @param bRandomAlpha If true, the Alpha channel is randomized.
	 * @return A randomized 8-bit (LDR) color.
	 */
	UFUNCTION(BlueprintPure, Category = "Math|Color")
		static FColor RandomColor(const bool bTrueRandom, const bool bRandomAlpha);
};
