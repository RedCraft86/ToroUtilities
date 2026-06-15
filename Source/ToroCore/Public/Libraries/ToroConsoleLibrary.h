// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "HAL/IConsoleManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroConsoleLibrary.generated.h"

/**
 * A utility library for managing Unreal Engine Console Variables (CVars) through Blueprints.
 * Provides a clean interface for getting and setting CVars of various types (Bool, Int, Float, String).
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroConsoleLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Sets the value of a Boolean console variable.
	 * @param InName  The exact name of the console variable (e.g., "r.Nanite").
	 * @param InValue The new boolean value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Bool)")
		static void SetCVarBool(const FString& InName, const bool InValue);

	/**
	 * Sets the value of an Integer console variable.
	 * @param InName  The exact name of the console variable.
	 * @param InValue The new integer value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Int)")
		static void SetCVarInt(const FString& InName, const int32 InValue);

	/**
	 * Sets the value of a Float console variable.
	 * @param InName  The exact name of the console variable.
	 * @param InValue The new float value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (float)")
		static void SetCVarFloat(const FString& InName, const float InValue);

	/**
	 * Sets the value of a String console variable.
	 * @param InName  The exact name of the console variable.
	 * @param InValue The new string value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (String)")
		static void SetCVarString(const FString& InName, const FString& InValue);

	/**
	 * Retrieves the current value of a Boolean console variable.
	 * @param InName  The exact name of the console variable.
	 * @return The current boolean value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Bool)")
		[[nodiscard]] static bool GetCVarBool(const FString& InName);

	/**
	 * Retrieves the current value of an Integer console variable.
	 * @param InName  The exact name of the console variable.
	 * @return The current integer value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Int)")
		[[nodiscard]] static int32 GetCVarInt(const FString& InName);

	/**
	 * Retrieves the current value of a Float console variable.
	 * @param InName  The exact name of the console variable.
	 * @return The current float value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (float)")
		[[nodiscard]] static float GetCVarFloat(const FString& InName);

	/**
	 * Retrieves the current value of a String console variable.
	 * @param InName  The exact name of the console variable.
	 * @return The current string value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (String)")
		[[nodiscard]] static FString GetCVarString(const FString& InName);

	/** 
	 * Locates a console variable in the engine's global manager.
	 * @param InName The name of the CVar to find.
	 * @return Pointer to the CVar interface, or nullptr if not found.
	 */
	[[nodiscard]] static IConsoleVariable* FindCVar(const FString& InName);

	/** The minimum priority flag used when setting CVar values. */
	static inline EConsoleVariableFlags MinPriority = ECVF_SetByScalability;

	/** The maximum priority flag used when setting CVar values. */
	static inline EConsoleVariableFlags MaxPriority = ECVF_SetByConsole;
};
