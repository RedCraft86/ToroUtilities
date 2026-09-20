// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#include "Widgets/GameVersionBox.h"
#include "GeneralProjectSettings.h"
#if WITH_EDITOR
#include "Widgets/ToroWidgetPalettes.h"
#endif

UGameVersionBox::UGameVersionBox()
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Font.Size = 24.0f;
	Font.TypefaceFontName = TEXT("Regular");
	Font.OutlineSettings.OutlineColor = FLinearColor::Black;
	Font.OutlineSettings.OutlineSize = 2;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	Justification = ETextJustify::Left;
}

TSharedRef<SWidget> UGameVersionBox::RebuildWidget()
{
	FString Result = TEXT("Version: ");
	if (const UGeneralProjectSettings* Settings = GetDefault<UGeneralProjectSettings>())
	{
		Result.Appendf(TEXT("%s-%s"), *Settings->ProjectVersion, *FString(LexToString(FApp::GetBuildConfiguration())).ToUpper());
		if (!Settings->ProjectDebugTitleInfo.IsEmptyOrWhitespace())
		{
			Result.Appendf(TEXT(" | %s"), *Settings->ProjectDebugTitleInfo.ToString());
		}
	}
	else
	{
		Result.Append("Unknown");
	}

	SetText(FText::FromString(Result));
	return Super::RebuildWidget();
}

#if WITH_EDITOR
const FText UGameVersionBox::GetPaletteCategory()
{
	return ToroWidgetPalettes::Information;
}
#endif