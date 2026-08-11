// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "CommonTextBlock.h"
#include "GameVersionBox.generated.h"

/**
 * Displays the game version in format where a debug title can be used to also specify a build name.
 * <pre>
 * {version}-{build type} | {debug title}
 * </pre>
 * For example:
 * <pre>
 * 1.0.0-SHIPPING | Exploration Demo
 * </pre>
 */
UCLASS(MinimalAPI, NotBlueprintable, BlueprintType)
class UGameVersionBox final : public UCommonTextBlock
{
	GENERATED_BODY()

public:

	UGameVersionBox();

private:

	virtual TSharedRef<SWidget> RebuildWidget() override;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
