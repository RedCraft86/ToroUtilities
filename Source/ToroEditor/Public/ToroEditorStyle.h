// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "SlateStyle/ToroSlateStyle.h"

class FToroEditorStyle final : public FToroSlateStyle
{
	TORO_SLATE_STYLE(FToroEditorModule, FToroEditorStyle, ToroEditorStyle)

public:

	virtual void AddResources() override
	{
		SetContentRoot(FToroEditorModule::GetPluginDirectory() / TEXT("Resources"));

		AddSVG(TEXT("Toolbar.RestartEd"), TEXT("Toolbar/RestartEd"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorSplit"), TEXT("Toolbar/ActorSplit"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorInstance"), TEXT("Toolbar/ActorInstance"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorLayout"), TEXT("Toolbar/ActorLayout"), Icon20x20);
		AddSVG(TEXT("Toolbar.ActorMerge"), TEXT("Toolbar/ActorMerge"), Icon20x20);

		AddPNG(TEXT("ClassThumbnail.LightProbeActor"), TEXT("Class/LightProbe"), Icon64x64);
	}
};