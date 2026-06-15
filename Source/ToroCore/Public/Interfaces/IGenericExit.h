// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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