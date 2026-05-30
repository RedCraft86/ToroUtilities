// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "SlateStyle/ToroSlateStyle.h"

class FToroEditorStyle final : public FToroSlateStyle
{
	TORO_SLATE_STYLE(FToroEditorModule, FToroEditorStyle, ToroEditor)

public:

	virtual void AddResources() override
	{
		SetContentRoot(FToroEditorModule::GetPluginDirectory() / TEXT("Resources"));
	}
};