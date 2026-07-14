// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserWidgets/VersionLabelBox.h"
#include "UObject/ConstructorHelpers.h"
#include "GeneralProjectSettings.h"
#include "Materials/Material.h"
#include "ToroSettings.h"
#include "Misc/App.h"

UVersionLabelBox::UVersionLabelBox()
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Font.Size = 21.0f;
	Font.TypefaceFontName = TEXT("Regular");
	Font.OutlineSettings.OutlineColor = FLinearColor::Black;
	Font.OutlineSettings.OutlineSize = 2;

	if (!IsRunningCommandlet() && !IsRunningDedicatedServer())
	{
		const ConstructorHelpers::FObjectFinder<UMaterial> FontMaterial(
		   TEXT("/ToroUtilities/Widgets/Assets/UIM_Gradient.UIM_Gradient"));
		if (FontMaterial.Succeeded()) Font.FontMaterial = FontMaterial.Object;
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	Justification = ETextJustify::Left;
}

TSharedRef<SWidget> UVersionLabelBox::RebuildWidget()
{
	FString Result;
	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		Result.Appendf(TEXT("Version: %s-%s"), *ProjectSettings->ProjectVersion,
			*FString(LexToString(FApp::GetBuildConfiguration())).ToUpper());
	}
	if (const FName DemoName = UToroSettings::Get()->DemoName; !DemoName.IsNone())
	{
		Result.Appendf(TEXT(" | %s"), *DemoName.ToString());
	}

	SetText(FText::FromString(Result));
	return Super::RebuildWidget();
}
