// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UObject/Interface.h"
#include "IGenericExit.generated.h"

/**
 * General use interface to run a universal "Exit" command on objects. (UObject Wrapper)
 */
UINTERFACE()
class UGenericExit : public UInterface
{
	GENERATED_BODY()
};

/**
 * General use interface to run a universal "Exit" command on objects.
 */
class TOROCORE_API IGenericExit
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = General)
		bool Exit(UObject* Instigator);
	virtual bool Exit_Implementation(UObject* Instigator) = 0;

	static bool Exit(UObject* Target, UObject* Instigator)
	{
		return Target && Target->Implements<UGenericExit>() 
			&& Execute_Exit(Target, Instigator);
	}
};