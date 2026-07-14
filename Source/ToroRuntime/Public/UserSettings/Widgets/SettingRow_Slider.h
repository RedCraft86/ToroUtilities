// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "SettingRowBase.h"
#include "SettingRow_Slider.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API USettingRow_Slider final : public USettingRowBase
{
	GENERATED_BODY()

public:

	USettingRow_Slider(const FObjectInitializer& ObjectInit);

protected:

	virtual void ConstructSettings(UHorizontalBox* Container) override;
};
