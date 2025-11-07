// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "UserSettings/OptionBindingBase.h"
#include "DeveloperBindings.generated.h"

USTRUCT(BlueprintType, DisplayName = "Developer Mode")
struct TORORUNTIME_API FDeveloperModeBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FDeveloperModeBinding()
	{
		Name = INVTEXT("Developer Mode");
		Tooltip = INVTEXT("Enables some developer options.");
	}

	virtual bool GetValue() const override { return GetSettings()->GetDeveloperMode(); }
	virtual void SetValue(const bool InValue) override
	{
		GetSettings()->SetDeveloperMode(InValue);
		GetSettings()->OnSettingsUpdated.Broadcast(ESettingApplyType::UI);
	}
};

USTRUCT(BlueprintType, DisplayName = "[Developer] Unlit Viewmode")
struct TORORUNTIME_API FDevUnlitModeBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FDevUnlitModeBinding()
	{
		Name = INVTEXT("Unlit Viewmode");
		Tooltip = INVTEXT("Render the game with no lighting. (Fullbright)");
	}

	virtual bool GetValue() const override
	{
		const UToroGameInstance* GI = GetGameInstance();
		return GI ? GI->IsUnlitViewmode() : false;
	}

	virtual void SetValue(const bool InValue) override
	{
		if (UToroGameInstance* GI = GetGameInstance())
		{
			GI->SetUnlitViewmode(InValue);
		}
	}
};

USTRUCT(BlueprintType, DisplayName = "[Developer] Invincible Player")
struct TORORUNTIME_API FDevInvincibilityBinding : public FToggleOptionBinding
{
	GENERATED_BODY()

	FDevInvincibilityBinding()
	{
		Name = INVTEXT("Invincible Player");
		Tooltip = INVTEXT("Player will not be seen by enemies and will be immune to attacks.");
	}

	virtual bool GetValue() const override
	{
		const UToroGameInstance* GI = GetGameInstance();
		return GI ? GI->IsPlayerInvincible() : false;
	}

	virtual void SetValue(const bool InValue) override
	{
		if (UToroGameInstance* GI = GetGameInstance())
		{
			GI->SetPlayerInvincible(InValue);
		}
	}
};