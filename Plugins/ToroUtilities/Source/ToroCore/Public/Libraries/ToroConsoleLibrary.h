// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToroConsoleLibrary.generated.h"

UCLASS(DisplayName = "Console Library")
class TOROCORE_API UToroConsoleLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Bool)")
		static void SetCVarBool(const FString& InName, const bool InValue);

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (Int)")
		static void SetCVarInt(const FString& InName, const int32 InValue);

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (float)")
		static void SetCVarFloat(const FString& InName, const float InValue);

	UFUNCTION(BlueprintCallable, Category = ConsoleVariables, DisplayName = "Set Console Variable (String)")
		static void SetCVarString(const FString& InName, const FString& InValue);
	
	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Bool)")
		static bool GetCVarBool(const FString& InName);

	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (Int)")
		static int32 GetCVarInt(const FString& InName);

	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (float)")
		static float GetCVarFloat(const FString& InName);

	UFUNCTION(BlueprintPure, Category = ConsoleVariables, DisplayName = "Get Console Variable (String)")
		static FString GetCVarString(const FString& InName);

	static IConsoleVariable* FindCVar(const FString& InName);

private:

	static TMap<FString, IConsoleVariable*> CachedCVars;
};
