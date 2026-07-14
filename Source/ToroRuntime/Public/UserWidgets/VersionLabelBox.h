// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Components/TextBlock.h"
#include "VersionLabelBox.generated.h"

/**
 * Displays the game version in format 
 * <pre>
 *     {version}-{build type} | {demo name}
 * </pre>
 * For example: 
 * <pre>
 *     1.0.0-SHIPPING | Exploration Demo
 * </pre>
 */
UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UVersionLabelBox final : public UTextBlock
{
	GENERATED_BODY()

public:

	UVersionLabelBox();

private:

	virtual TSharedRef<SWidget> RebuildWidget() override;
};
