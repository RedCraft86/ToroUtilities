// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "CommonTextBlock.h"
#include "DataTypes/CachedGetter.h"
#include "GameFramework/GameUserSettings.h"
#include "FrameRateBox.generated.h"

/**
 * Displays averaged frame rate and frame time in milliseconds.
 * Interpolates between performance colors using the configured frame rate limit.
 * <pre>
 * 120 FPS
 * 8.33 ms
 * </pre>
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class UFrameRateBox final : public UCommonTextBlock
{
	GENERATED_BODY()

public:

	UFrameRateBox();

private:

	/**
	 * Color used at the low end of the frame rate range.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (AllowPrivateAccess = true))
		FLinearColor LowPerformanceColor;

	/**
	 * Color used at the frame rate cap, or at 60 FPS when uncapped.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (AllowPrivateAccess = true))
		FLinearColor HighPerformanceColor;

	TWeakPtr<FActiveTimerHandle> TickHandle;
	FNumberFormattingOptions FrameRateDisplayOptions;
	FNumberFormattingOptions FrameTimeDisplayOptions;

	TCachedGetter<UGameUserSettings> UserSettings{[](){
		return GEngine ? GEngine->GetGameUserSettings() : nullptr;
	}};

	void ClearActiveTimer();
	EActiveTimerReturnType Update(double CurrentTime, float DeltaTime);

	virtual void BeginDestroy() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
	virtual bool CanEditChange(const FProperty* InProperty) const override;
#endif
};
