// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Widgets/FrameRateBox.h"
#if WITH_EDITOR
#include "Misc/App.h"
#endif

UFrameRateBox::UFrameRateBox(): bInit(false), bShowingFPS(false)
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Font.TypefaceFontName = TEXT("Regular");
	Font.OutlineSettings.OutlineColor = FLinearColor::Black;
	Font.OutlineSettings.OutlineSize = 2;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	Justification = ETextJustify::Right;

	FPSOptions.MaximumFractionalDigits = 0;
	FPSOptions.MinimumFractionalDigits = 0;
	MSOptions.MaximumFractionalDigits = 1;
	MSOptions.MinimumFractionalDigits = 1;
}

EActiveTimerReturnType UFrameRateBox::Update(double CurrentTime, float DeltaTime)
{
	if (UserSettings.Get())
	{
		if (!bInit || bShowingFPS != UserSettings->GetShowFPS())
		{
			bInit = true;
			bShowingFPS = UserSettings->GetShowFPS();
			SetVisibility(bShowingFPS ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}

		if (bShowingFPS)
		{
			const float CurrentFPS = UserSettings->GetAverageFPS();

			SetColorAndOpacity(FMath::Lerp(FLinearColor::Red, FLinearColor::Green, 
				FMath::Clamp(CurrentFPS / FMath::Max(UserSettings->GetFrameRateLimit(), 0.01f), 0.0f, 1.0f)));

			SetText(FText::Format(INVTEXT("{0} FPS\n{1} ms"),
				FText::AsNumber(CurrentFPS, &FPSOptions), 
				FText::AsNumber(UserSettings->GetAverageMS(), &MSOptions)));
		}
	}
	else if (GetVisibility() != ESlateVisibility::Collapsed)
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

	return EActiveTimerReturnType::Continue;
}

TSharedRef<SWidget> UFrameRateBox::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	Widget->RegisterActiveTimer(0.1f, FWidgetActiveTimerDelegate
		::CreateUObject(this, &UFrameRateBox::Update)
	);
	return Widget;
}
