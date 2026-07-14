// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "UserSettings/Widgets/SettingRow_Toggle.h"

USettingRow_Toggle::USettingRow_Toggle(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_Bool::StaticStruct();
}

void USettingRow_Toggle::ConstructSettings(UHorizontalBox* Container)
{
	Super::ConstructSettings(Container);
}
