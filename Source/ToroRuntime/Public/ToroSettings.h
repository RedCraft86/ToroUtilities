// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UObject/SoftObjectPtr.h"
#include "DataAssets/ToroDatabase.h"
#include "Engine/DeveloperSettings.h"
#include "ToroSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, DisplayName = "ToroUtilities")
class TORORUNTIME_API UToroSettings final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroSettings();

	UFUNCTION(BlueprintPure, Category = Game, DisplayName = "Get ToroUtilities Settings")
	[[nodiscard]] static const UToroSettings* Get()
	{
		return GetDefault<UToroSettings>();
	}

	/**
	 * If the game is a Demo, name of the demo to be appended on the version string.
	 */
	UPROPERTY(Config, EditAnywhere, Category = Game)
		FName DemoName;

	/**
	 * Global list for the game's database assets. Use GetDatabase<UType>() to obtain one.
	 */
	UPROPERTY(Config, EditAnywhere, Category = Game)
		TSet<TSoftObjectPtr<UToroDatabase>> Databases;

	/** 
	 * The Master Widget class to spawn for the Player HUD. 
	 */
	UPROPERTY(Config, EditAnywhere, Category = UserWidgets)
		TSoftClassPtr<class UToroMasterWidget> MasterWidgetClass;

	/** 
	 * The User Dialog Widget class to use when creating user confirmation dialogs. 
	 */
	UPROPERTY(Config, EditAnywhere, Category = UserWidgets)
		TSoftClassPtr<class UToroUserDialog> UserDialogClass;

	/**
	 * The Settings Widget class to use when creating a settings screen.
	 */
	UPROPERTY(Config, EditAnywhere, Category = UserWidgets)
		TSoftClassPtr<class UToroSettingsWidget> SettingsWidgetClass;

	/**
	 * Finds the database asset of the specified class in the global list.
	 * @param Class Database to look for in the global list.
	 * @return Database asset or nullptr if not found.
	 */
	UFUNCTION(BlueprintPure, Category = Game, meta = (DeterminesOutputType = Class))
		UToroDatabase* GetDatabase(UPARAM(meta=(AllowAbstract=false)) const TSubclassOf<UToroDatabase> Class) const;

	/**
	 * Finds the database asset of the specified class in the global list.
	 * @tparam T Database class to look for. Must derive from UToroDatabase.
	 * @return Database asset or nullptr if not found.
	 */
	template<typename T>
	T* GetDatabase() const
	{
		static_assert(TIsDerivedFrom<T, UToroDatabase>::IsDerived, "T must derive from UToroDatabase");
		return Cast<T>(GetDatabase(T::StaticClass()));
	}
};
