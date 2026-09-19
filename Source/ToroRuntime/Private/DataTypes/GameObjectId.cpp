// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "DataTypes/GameObjectId.h"

const FGameObjectId FGameObjectId::EmptyId = FGameObjectId();

void FGameObjectId::Invalidate()
{
	Group = FGameplayTag::EmptyTag;
	Guid.Invalidate();
}

void FGameObjectId::RegenerateGuid(const bool bOnlyIfInvalid)
{
	if (!bOnlyIfInvalid || !Guid.IsValid())
	{
		Guid = FGuid::NewGuid();
	}
}
