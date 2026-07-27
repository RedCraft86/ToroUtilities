// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#pragma once

#include "UObject/Interface.h"
#include "IExitable.generated.h"

/**
 * General use interface to add an arbitrary "Exit" function to objects. (UObject Wrapper)
 */
UINTERFACE()
class UExitable : public UInterface
{
	GENERATED_BODY()
};

/**
 * General use interface to add an arbitrary "Exit" function to objects.
 */
class TOROCORE_API IExitable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = ToroCore)
		bool Exit(const UObject* Instigator);

	static bool RequestExit(UObject* Target, const UObject* Instigator)
	{
		return Target && Target->Implements<UExitable>() && Execute_Exit(Target, Instigator);
	}
};
