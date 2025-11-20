// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToroCommands.h"
#include "Framework/Commands/Commands.h"

class FActorBaker
{
public:

	static void Register(const TSharedPtr<FUICommandList>& Commands)
	{
		MAP_TOOL(ActorBaker, FActorBaker);
	}
	
	static void RegisterMenus(const TSharedPtr<FUICommandList>& Commands)
	{
		REGISTER_TOP_MENU(ActorBaker)
		REGISTER_TOOLBAR(ActorBaker, Actor)
	}

private:

	static void ExecuteAction();
};