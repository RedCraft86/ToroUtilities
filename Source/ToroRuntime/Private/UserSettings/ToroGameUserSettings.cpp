// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserSettings/ToroGameUserSettings.h"

void UToroGameUserSettings::AutoAdjustScalability()
{
	RunHardwareBenchmark();
	ApplyHardwareBenchmarkResults();
	RequestUIUpdate();
}

void UToroGameUserSettings::InitializeSettings()
{
	LoadSettings(true);
	if (!bInitialized)
	{
		bInitialized = true;
		SetScreenResolution(GetDesktopResolution());
		SetFullscreenMode(EWindowMode::WindowedFullscreen);
	}
	ApplySettings(false);
}
