// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "UObject/Interface.h"
#include "Components/ActorComponent.h"
#include "IEnabledState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableStateDelegateBP, const bool, bState);
DECLARE_MULTICAST_DELEGATE_OneParam(FEnableStateDelegate, const bool);

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

	/**
	 * Sets whether this object should be enabled.
	 * @param bEnabled Target enabled state.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = EnabledState)
		void SetEnabledState(const bool bEnabled);

	/**
	 * Gets if this object is currently enabled.
	 * @return True if object is enabled.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = EnabledState)
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
