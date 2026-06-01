// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "DataTypes/GameObjectId.h"

const FGameObjectId& FGameObjectId::GetEmpty()
{
	static const FGameObjectId EmptyId;
	return EmptyId;
}

void FGameObjectId::Invalidate()
{
	Group = FGameplayTag::EmptyTag;
	Guid.Invalidate();
}

void FGameObjectId::RegenerateGuid(const bool bOnlyIfInvalid)
{
	if (!Guid.IsValid() || !bOnlyIfInvalid)
	{
		Guid = FGuid::NewGuid();
	}
}
