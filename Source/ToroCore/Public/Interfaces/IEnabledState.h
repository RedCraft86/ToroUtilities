// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UObject/Interface.h"
#include "IEnabledState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableStateDelegateBP, const bool, bState);
DECLARE_MULTICAST_DELEGATE_OneParam(FEnableStateDelegate, const bool);

/**
 * General use interface to get/set the enabled state of objects. (UObject Wrapper)
 */
UINTERFACE()
class UEnabledState : public UInterface
{
	GENERATED_BODY()
};

/**
 * General use interface to get/set the enabled state of objects.
 */
class TOROCORE_API IEnabledState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = General)
		void SetEnabledState(const bool bInEnabled);
	virtual void SetEnabledState_Implementation(const bool bInEnabled) = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = General)
		bool GetEnabledState() const;
	virtual bool GetEnabledState_Implementation() const = 0;

	static void SetEnabled(UObject* Target, const bool bInEnabled)
	{
		if (Target && Target->Implements<UEnabledState>())
		{
			Execute_SetEnabledState(Target, bInEnabled);
		}
	}

	static bool IsEnabled(const UObject* Target)
	{
		return Target && (!Target->Implements<UEnabledState>() || Execute_GetEnabledState(Target));
	}
};