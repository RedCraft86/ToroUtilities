// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#pragma once

#include "UObject/Interface.h"
#include "Components/ActorComponent.h"
#include "IEnabledState.generated.h"

/**
 * General use interface to add an arbitrary Enabled State functions to objects. (UObject Wrapper)
 */
UINTERFACE()
class UEnabledState : public UInterface
{
	GENERATED_BODY()
};

/**
 * General use interface to add an arbitrary Enabled State functions to objects.
 */
class TOROCORE_API IEnabledState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ToroCore)
		void SetEnabledState(const bool bEnabled);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ToroCore)
		bool GetEnabledState() const;

	static void SetEnabled(UObject* Target, const bool bEnabled)
	{
		if (Target && !Target->Implements<UEnabledState>())
		{
			Execute_SetEnabledState(Target, bEnabled);
		}
	}

	static bool IsEnabled(const UObject* Target)
	{
		return Target && (!Target->Implements<UEnabledState>() || Execute_GetEnabledState(Target));
	}
};
