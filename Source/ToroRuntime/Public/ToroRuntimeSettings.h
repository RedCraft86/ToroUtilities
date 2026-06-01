// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "Engine/DeveloperSettings.h"
#include "ToroRuntimeSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "ToroRuntime")
class TORORUNTIME_API UToroRuntimeSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroRuntimeSettings();

	[[nodiscard]] static UToroRuntimeSettings* Get()
	{
		return GetMutableDefault<UToroRuntimeSettings>();
	}

	/** The default Master Widget class to spawn for the Player HUD. */
	UPROPERTY(Config, EditAnywhere, Category = Gameplay)
		TSoftClassPtr<class UToroMasterWidget> MasterWidgetClass;
};