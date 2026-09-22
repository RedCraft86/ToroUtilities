// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ToroSlateStyle.h"

class FToroEditorStyle final : public FToroSlateStyle
{
	TORO_SLATE_STYLE(FToroEditorModule, FToroEditorStyle, ToroEditorStyle)

public:

	virtual void AddResources() override
	{
		SetContentRoot(FToroEditorModule::GetPluginDir() / TEXT("Resources"));
		// TODO: add slate icons
	}
};
