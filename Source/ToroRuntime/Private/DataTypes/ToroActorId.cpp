// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "DataTypes/ToroActorId.h"

const FToroActorId FToroActorId::EmptyId = FToroActorId();

void FToroActorId::Invalidate()
{
	Group = FGameplayTag::EmptyTag;
	Guid.Invalidate();
}

void FToroActorId::RegenerateGuid(const bool bOnlyIfInvalid)
{
	if (!bOnlyIfInvalid || !Guid.IsValid())
	{
		Guid = FGuid::NewGuid();
	}
}
