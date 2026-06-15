// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "Engine/DeveloperSettings.h"
#include "ToroUtilitiesSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "ToroUtilities")
class TORORUNTIME_API UToroUtilitiesSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroUtilitiesSettings();

	[[nodiscard]] static const UToroUtilitiesSettings* Get()
	{
		return GetDefault<UToroUtilitiesSettings>();
	}

	/** The Master Widget class to spawn for the Player HUD. */
	UPROPERTY(Config, EditAnywhere, Category = UserWidgets)
		TSoftClassPtr<class UToroMasterWidget> MasterWidgetClass;

	/** The User Dialog Widget class to use when creating user confirmation dialogs. */
	UPROPERTY(Config, EditAnywhere, Category = UserWidgets)
		TSoftClassPtr<class UToroUserDialog> UserDialogClass;
};
