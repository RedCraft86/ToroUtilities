// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "Engine/DeveloperSettings.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "ToroUtilities")
class TORORUNTIME_API UToroSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings();

	[[nodiscard]] static const UToroSettings* Get()
	{
		return GetDefault<UToroSettings>();
	}

	/** The Master Widget class to spawn for the Player HUD. */
	UPROPERTY(Config, EditAnywhere, Category = UserWidgets)
		TSoftClassPtr<class UToroMasterWidget> MasterWidgetClass;

	/** The User Dialog Widget class to use when creating user confirmation dialogs. */
	UPROPERTY(Config, EditAnywhere, Category = UserWidgets)
		TSoftClassPtr<class UToroUserDialog> UserDialogClass;
};
