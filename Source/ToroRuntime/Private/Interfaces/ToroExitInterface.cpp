// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Interfaces/ToroExitInterface.h"

bool IToroExitInterface::RequestExit(UObject* Target, const UObject* Instigator)
{
	return IsValid(Target)
		&& Target->Implements<UToroExitInterface>()
		&& Execute_RequestExit(Target, Instigator);
}
