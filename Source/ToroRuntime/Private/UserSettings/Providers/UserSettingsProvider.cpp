// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserSettings/Providers/UserSettingsProvider.h"

FUserSettingsProviderBase::FUserSettingsProviderBase()
	: DisplayName(INVTEXT("Setting")), Description(FText::GetEmpty())
	, Performance(EUserSettingImpact::None), bUpdateDynamically(false)
{
}

FText FUserSettingsProviderBase::GetFormattedTooltip() const
{
	FTextBuilder Builder;
	Builder.AppendLine(Description);
	if (Performance != EUserSettingImpact::None)
	{
		Builder.AppendLineFormat(INVTEXT("\nImpact: {0}"), GetPerformanceLabel());
	}

	return Builder.ToText();
}

const FText& FUserSettingsProviderBase::GetPerformanceLabel() const
{
	static const FText LABELS[] = {
		FText::GetEmpty(),
		INVTEXT("Low"),
		INVTEXT("Medium"),
		INVTEXT("High"),
		INVTEXT("Varies")
	};

	return LABELS[static_cast<uint8>(Performance)];
}

float FUserSettingsProvider_Float::GetValueDelta() const
{
	static constexpr float DELTAS[] = {
		1.00f,	// 0 decimal
		0.10f,	// 1 decimal
		0.01f	// 2 decimals
	};
	return DELTAS[NumDecimals];
}

#if WITH_EDITOR
void FUserSettingsProvider_Float::UpdateProvider()
{
	if (MinValue > MaxValue)
	{
		MinValue = MaxValue - GetValueDelta();
	}

	if (DefaultOption > MaxValue)
	{
		DefaultOption = MaxValue;
	}
	else if (DefaultOption < MinValue)
	{
		DefaultOption = MinValue;
	}
}

void FUserSettingsProvider_IntSwap::UpdateProvider()
{
	if (OptionNames.IsEmpty())
	{
		OptionNames.Add(TEXT("None"));
	}
	if (DefaultOption > OptionNames.Num())
	{
		DefaultOption = OptionNames.Num() - 1;
	}
}
#endif
