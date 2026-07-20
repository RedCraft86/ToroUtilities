// Copyright (C) 2026 Tayzar Linn. Licensed under LGPL-3.0, see project LICENSE file.

#include "DataTypes/GameObjectId.h"
#include "Interfaces/IIdentifiable.h"

const FGameObjectId FGameObjectId::EmptyId = FGameObjectId();

FGameObjectId FGameObjectId::Get(const UObject* Target)
{
	return IIdentifiable::Get(Target);
}

void FGameObjectId::Invalidate()
{
	Group = FGameplayTag::EmptyTag;
	Guid.Invalidate();
}

void FGameObjectId::RegenerateGuid(const bool bOnlyIfInvalid)
{
	if (bOnlyIfInvalid || !Guid.IsValid())
	{
		Guid = FGuid::NewGuid();
	}
}
