// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Interfaces/ToroActorInterface.h"

namespace
{
	bool ImplementedBy(const AActor* Target)
	{
		return IsValid(Target) && Target->Implements<UToroActorInterface>();
	}
}

FToroActorId IToroActorInterface::GetIdentity(const AActor* Target)
{
	return ImplementedBy(Target) ? Execute_GetIdentity(Target) : FToroActorId::EmptyId;
}

bool IToroActorInterface::GetActiveState(const AActor* Target)
{
	return ImplementedBy(Target) && Execute_GetActiveState(Target);
}

void IToroActorInterface::SetActiveState(AActor* Target, const bool bActive)
{
	if (ImplementedBy(Target))
	{
		Execute_SetActiveState(Target, bActive);
	}
}
