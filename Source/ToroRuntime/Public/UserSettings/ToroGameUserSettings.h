// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Engine/Engine.h"
#include "GameFramework/GameUserSettings.h"
#include "ToroGameUserSettings.generated.h"

extern ENGINE_API float GAverageMS;
extern ENGINE_API float GAverageFPS;

UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:

	UToroGameUserSettings()
		: bInitialized(false)
	{}

	template<typename T = UToroGameUserSettings>
	[[nodiscard]] static T* Get()
	{
		return GEngine ? Cast<T>(GEngine->GetGameUserSettings()) : nullptr;
	}

	UFUNCTION(BlueprintPure, Category = Settings)
	static float GetAverageMS()
	{
		return GAverageMS;
	}

	UFUNCTION(BlueprintPure, Category = Settings)
	static float GetAverageFPS()
	{
		//return ImageFidelityAPI::XeSS::UseXeFGFrameRate() ? GXeFGAverageFPS : GAverageFPS;
		return GAverageFPS;
	}

	UFUNCTION(BlueprintCallable, Category = Settings)
		void AutoAdjustScalability();

	void InitializeSettings();

protected:

	UPROPERTY(Config) bool bInitialized;
};
