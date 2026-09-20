// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroCVarLibrary.generated.h"

/**
 * Provides typed Blueprint access to registered console variables.
 */
UCLASS(NotBlueprintable, NotBlueprintType)
class TORORUNTIME_API UToroCVarLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static constexpr EConsoleVariableFlags MinPriority = ECVF_SetByScalability;
	static constexpr EConsoleVariableFlags MaxPriority = ECVF_SetByConsole;

	/**
	 * Sets a registered console variable to a bool value if its priority permits.
	 * @param InName Name of the console variable.
	 * @param InValue Value to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Bool)")
		static void SetCVarBool(const FString& InName, const bool InValue);

	/**
	 * Sets a registered console variable to an int value if its priority permits.
	 * @param InName Name of the console variable.
	 * @param InValue Value to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Int)")
		static void SetCVarInt(const FString& InName, const int32 InValue);

	/**
	 * Sets a registered console variable to a float value if its priority permits.
	 * @param InName Name of the console variable.
	 * @param InValue Value to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (float)")
		static void SetCVarFloat(const FString& InName, const float InValue);

	/**
	 * Sets a registered console variable to a string value if its priority permits.
	 * @param InName Name of the console variable.
	 * @param InValue Value to assign.
	 */
	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (String)")
		static void SetCVarString(const FString& InName, const FString& InValue);

	/**
	 * Reads a registered console variable as a bool value.
	 * @param InName Name of the console variable.
	 * @param bInDefault Value returned when the variable does not exist.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Bool)")
		static bool GetCVarBool(const FString& InName, const bool bInDefault = false);

	/**
	 * Reads a registered console variable as an int value.
	 * @param InName Name of the console variable.
	 * @param InDefault Value returned when the variable does not exist.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Int)")
		static int32 GetCVarInt(const FString& InName, const int32 InDefault = 0);

	/**
	 * Reads a registered console variable as a float value.
	 * @param InName Name of the console variable.
	 * @param InDefault Value returned when the variable does not exist.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (float)")
		static float GetCVarFloat(const FString& InName, const float InDefault = 0.0f);

	/**
	 * Reads a registered console variable as a string value.
	 * @param InName Name of the console variable.
	 * @param InDefault Value returned when the variable does not exist.
	 */
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (String)")
		static FString GetCVarString(const FString& InName, const FString& InDefault = FString());

	static IConsoleVariable* FindCVar(const FString& InName);
};
