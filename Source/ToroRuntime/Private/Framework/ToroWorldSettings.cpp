// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Framework/ToroWorldSettings.h"
#include "FlowComponent.h"

AToroWorldSettings::AToroWorldSettings()
{
	if (UFlowComponent* FlowComp = GetFlowComponent())
	{
		FlowComp->bAutoStartRootFlow = false;
	}
}
