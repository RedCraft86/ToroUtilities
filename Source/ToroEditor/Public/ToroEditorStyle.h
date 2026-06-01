// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "SlateStyle/ToroSlateStyle.h"

class FToroEditorStyle final : public FToroSlateStyle
{
	TORO_SLATE_STYLE(FToroEditorModule, FToroEditorStyle, ToroEditorStyle)

public:

	virtual void AddResources() override
	{
		SetContentRoot(FToroEditorModule::GetPluginDirectory() / TEXT("Resources"));

		AddSVG(TEXT("Toolbar.Restart"), TEXT("Toolbar/Restart"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorBake"), TEXT("Toolbar/ActorBake"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorInstance"), TEXT("Toolbar/ActorInstance"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorLayout"), TEXT("Toolbar/ActorLayout"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorMerge"), TEXT("Toolbar/ActorMerge"), Icon20x20);

		AddPNG(TEXT("ClassThumbnail.LightProbeActor"), TEXT("Class/LightProbe"), Icon64x64);
	}
};