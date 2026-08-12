// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "UObject/Interface.h"
#include "DataTypes/GameObjectId.h"
#include "IIdentifiable.generated.h"

/**
 * General use interface to access FObjectIdentity from objects. (UObject Wrapper)
 */
UINTERFACE()
class UIdentifiable : public UInterface
{
	GENERATED_BODY()
};

/**
 * General use interface to access FObjectIdentity from objects.
 */
class TOROCORE_API IIdentifiable
{
	GENERATED_BODY()

public:

	/**
	 * Gets the identifier for this object that can be used in save systems or character searching.
	 * @return The unique identifier for this object.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Object)
		FGameObjectId GetIdentity() const;

	static FGameObjectId Get(const UObject* Target)
	{
		return Target && Target->Implements<UIdentifiable>() ? Execute_GetIdentity(Target) : FGameObjectId::EmptyId;
	}
};
