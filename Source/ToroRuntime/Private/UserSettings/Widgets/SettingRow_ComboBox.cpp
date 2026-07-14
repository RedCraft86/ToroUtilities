// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "UserSettings/Widgets/SettingRow_ComboBox.h"

USettingRow_ComboBox::USettingRow_ComboBox(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_String::StaticStruct();
}

void USettingRow_ComboBox::ConstructSettings(UHorizontalBox* Container)
{
	Super::ConstructSettings(Container);
}
