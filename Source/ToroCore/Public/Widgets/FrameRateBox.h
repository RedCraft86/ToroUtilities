// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "CommonTextBlock.h"
#include "DataTypes/CachedGetter.h"
#include "FrameRateBox.generated.h"

/**
 * Displays the game's frame rate and frame time (in ms).
 * Allows custom coloring based on the max frame rate set.
 * Format:
 * <pre>
 *     120 FPS
 *     8.33 ms
 * </pre>
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class UFrameRateBox final : public UCommonTextBlock
{
	GENERATED_BODY()

public:

	UFrameRateBox();

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (AllowPrivateAccess = true))
		FLinearColor LowPerformanceColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (AllowPrivateAccess = true))
		FLinearColor HighPerformanceColor;

	TWeakPtr<FActiveTimerHandle> TickHandle;
	FNumberFormattingOptions FrameRateDisplayOptions;
	FNumberFormattingOptions FrameTimeDisplayOptions;

	TCachedGetter<UGameUserSettings> UserSettings{[]()
	{
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
