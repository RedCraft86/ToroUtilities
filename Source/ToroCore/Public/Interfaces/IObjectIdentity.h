// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

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