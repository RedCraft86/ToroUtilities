// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "UserSettings/Widgets/SettingRow_Slider.h"

USettingRow_Slider::USettingRow_Slider(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_Float::StaticStruct();
}

void USettingRow_Slider::ConstructSettings(UHorizontalBox* Container)
{
	Super::ConstructSettings(Container);
}
