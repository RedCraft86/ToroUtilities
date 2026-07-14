// Copyright (C) 2026 Tayzar Linn. All Rights Reserved.

#pragma once

#include "SettingRowBase.h"
#include "SettingRow_ComboBox.generated.h"

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API USettingRow_ComboBox final : public USettingRowBase
{
	GENERATED_BODY()

public:

	USettingRow_ComboBox(const FObjectInitializer& ObjectInit);

protected:

	virtual void ConstructSettings(UHorizontalBox* Container) override;
};
