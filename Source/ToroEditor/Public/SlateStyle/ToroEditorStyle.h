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

		AddSVG(TEXT("Toolbar.RestartEd"), TEXT("Toolbar/RestartEd"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorSplit"), TEXT("Toolbar/ActorSplit"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorInstance"), TEXT("Toolbar/ActorInstance"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorLayout"), TEXT("Toolbar/ActorLayout"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorMerge"), TEXT("Toolbar/ActorMerge"), Icon20x20);

		AddPNG(TEXT("ClassThumbnail.LightProbeActor"), TEXT("Class/LightProbe"), Icon64x64);
	}
};
