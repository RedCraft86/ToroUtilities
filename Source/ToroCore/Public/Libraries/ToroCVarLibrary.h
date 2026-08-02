// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroCVarLibrary.generated.h"

/**
 * A utility library providing functions for Console Variables.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TOROCORE_API UToroCVarLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	 * Sets the value of a Boolean console variable.
	 * @param InName The exact name of the console variable (e.g., "r.Nanite").
	 * @param InValue The new boolean value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Bool)")
		static void SetCVarBool(const FString& InName, const bool InValue);

	/**
	 * Sets the value of an Integer console variable.
	 * @param InName The exact name of the console variable.
	 * @param InValue The new integer value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Int)")
		static void SetCVarInt(const FString& InName, const int32 InValue);

	/**
	 * Sets the value of a Float console variable.
	 * @param InName The exact name of the console variable.
	 * @param InValue The new float value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (float)")
		static void SetCVarFloat(const FString& InName, const float InValue);

	/**
	 * Sets the value of a String console variable.
	 * @param InName The exact name of the console variable.
	 * @param InValue The new string value to apply.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (String)")
		static void SetCVarString(const FString& InName, const FString& InValue);

	/**
	 * Retrieves the current value of a Boolean console variable.
	 * @param InName The exact name of the console variable.
	 * @param bInDefault Value to return if the console variable doesn't exist.
	 * @return The current boolean value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Bool)")
		static bool GetCVarBool(const FString& InName, const bool bInDefault = false);

	/**
	 * Retrieves the current value of an Integer console variable.
	 * @param InName The exact name of the console variable.
	 * @param InDefault Value to return if the console variable doesn't exist.
	 * @return The current integer value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Int)")
		static int32 GetCVarInt(const FString& InName, const int32 InDefault = 0);

	/**
	 * Retrieves the current value of a Float console variable.
	 * @param InName The exact name of the console variable.
	 * @param InDefault Value to return if the console variable doesn't exist.
	 * @return The current float value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (float)")
		static float GetCVarFloat(const FString& InName, const float InDefault = 0.0f);

	/**
	 * Retrieves the current value of a String console variable.
	 * @param InName The exact name of the console variable.
	 * @param InDefault Value to return if the console variable doesn't exist.
	 * @return The current string value of the CVar.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (String)")
		static FString GetCVarString(const FString& InName, const FString& InDefault = FString());

	/**
	 * Locates a console variable in the engine's global manager.
	 * @param InName The name of the CVar to find.
	 * @return Pointer to the CVar interface or nullptr if not found.
	 */
	static IConsoleVariable* FindCVar(const FString& InName);

	/**
	 * The minimum priority flag used when setting CVar values.
	 */
	static constexpr EConsoleVariableFlags MinPriority = ECVF_SetByScalability;

	/**
	 * The maximum priority flag used when setting CVar values.
	 */
	static constexpr EConsoleVariableFlags MaxPriority = ECVF_SetByConsole;
};
