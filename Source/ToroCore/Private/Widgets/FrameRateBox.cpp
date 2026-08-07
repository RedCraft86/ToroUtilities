// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "Widgets/FrameRateBox.h"
#include "GameFramework/GameUserSettings.h"
#if WITH_EDITOR
#include "Widgets/ToroWidgetPaletteCategories.h"
#endif

extern ENGINE_API float GAverageFPS;
extern ENGINE_API float GAverageMS;

UFrameRateBox::UFrameRateBox()
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Font.Size = 24.0f;
	Font.TypefaceFontName = TEXT("Regular");
	Font.OutlineSettings.OutlineColor = FLinearColor::Black;
	Font.OutlineSettings.OutlineSize = 2;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	Justification = ETextJustify::Right;

	LowPerformanceColor = FLinearColor::Red;
	HighPerformanceColor = FLinearColor::Green;
	FrameRateDisplayOptions.MinimumFractionalDigits = 0;
	FrameRateDisplayOptions.MaximumFractionalDigits = 0;
	FrameTimeDisplayOptions.MinimumFractionalDigits = 2;
	FrameTimeDisplayOptions.MaximumFractionalDigits = 2;
}

void UFrameRateBox::ClearActiveTimer()
{
	const TSharedPtr<SWidget> CurrentWidget = GetCachedWidget();
	if (CurrentWidget.IsValid() && TickHandle.IsValid())
	{
		CurrentWidget->UnRegisterActiveTimer(TickHandle.Pin().ToSharedRef());
	}

	TickHandle.Reset();
}

EActiveTimerReturnType UFrameRateBox::Update(double CurrentTime, float DeltaTime)
{
	if (UserSettings.Get()
		&& GetVisibility() != ESlateVisibility::Hidden
		&& GetVisibility() != ESlateVisibility::Collapsed)
	{
		const float FPSLimit = UserSettings->GetFrameRateLimit();
		SetColorAndOpacity(FMath::Lerp(LowPerformanceColor, HighPerformanceColor,
			FMath::GetMappedRangeValueClamped(
				FVector2D(10.0f, (FPSLimit > 0.0f) ? FPSLimit : 60.0f),
				FVector2D(0.0f, 1.0f),
				GAverageFPS
			)
		));

		SetText(FText::Format(INVTEXT("{0} FPS\n{1} ms"),
			FText::AsNumber(GAverageFPS, &FrameRateDisplayOptions),
			FText::AsNumber(GAverageMS, &FrameTimeDisplayOptions)
		));
	}

	return EActiveTimerReturnType::Continue;
}

void UFrameRateBox::BeginDestroy()
{
	ClearActiveTimer();
	Super::BeginDestroy();
}

TSharedRef<SWidget> UFrameRateBox::RebuildWidget()
{
	ClearActiveTimer();
	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	TickHandle = Widget->RegisterActiveTimer(0.1f, FWidgetActiveTimerDelegate
		::CreateUObject(this, &UFrameRateBox::Update)
	);
	return Widget;
}

#if WITH_EDITOR
const FText UFrameRateBox::GetPaletteCategory()
{
	return ToroWidgetPaletteCategories::Information;
}

bool UFrameRateBox::CanEditChange(const FProperty* InProperty) const
{
	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	if (InProperty && InProperty->GetName() == GET_MEMBER_NAME_CHECKED(UTextBlock, ColorAndOpacity))
	{
		return false;
	}
	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	return Super::CanEditChange(InProperty);
}
#endif