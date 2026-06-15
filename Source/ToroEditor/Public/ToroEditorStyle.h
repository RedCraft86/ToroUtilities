// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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