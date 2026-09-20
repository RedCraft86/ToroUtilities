// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "CommonTextBlock.h"
#include "GameVersionBox.generated.h"

/**
 * Displays the project version, build configuration, and optional debug title.
 * <pre>
 * Version: {version}-{build configuration} | {debug title}
 * </pre>
 * For example:
 * <pre>
 * Version: 1.0.0-SHIPPING | Exploration Demo
 * </pre>
 * The debug title and its separator are omitted when the title is empty.
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
