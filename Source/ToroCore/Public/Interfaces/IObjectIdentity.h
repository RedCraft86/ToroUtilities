// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UObject/Interface.h"
#include "DataTypes/GameObjectId.h"
#include "IObjectIdentity.generated.h"

/**
 * General use interface to access GameObjectId from objects. (UObject Wrapper)
 */
UINTERFACE()
class UObjectIdentity : public UInterface
{
	GENERATED_BODY()
};

/**
 * General use interface to access GameObjectId from objects.
 */
class TOROCORE_API IObjectIdentity
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = General)
		FGameObjectId GetGameObjectId() const;
	virtual FGameObjectId GetGameObjectId_Implementation() const = 0;
	
	static FGameObjectId Get(const UObject* Target)
	{
		return (Target && Target->Implements<UObjectIdentity>()) 
			? Execute_GetGameObjectId(Target) : FGameObjectId::GetEmpty();
	}
};