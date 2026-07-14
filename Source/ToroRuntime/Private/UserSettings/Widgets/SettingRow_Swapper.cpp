// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#include "UserSettings/Widgets/SettingRow_Swapper.h"

USettingRow_Swapper::USettingRow_Swapper(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_IntSwap::StaticStruct();
}

void USettingRow_Swapper::ConstructSettings(UHorizontalBox* Container)
{
	Super::ConstructSettings(Container);
}
