// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "ToroSettings.h"
#include "GeneralProjectSettings.h"
#include "Misc/App.h"

UToroSettings::UToroSettings()
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroUtilities");

	MasterWidgetClass = FSoftClassPath(TEXT("/ToroUtilities/Widgets/WBP_MasterWidget.WBP_MasterWidget_C"));
	UserDialogClass = FSoftClassPath(TEXT("/ToroUtilities/Widgets/WBP_UserDialog.WBP_UserDialog_C"));
	SettingsWidgetClass = FSoftClassPath(TEXT("/ToroUtilities/Widgets/WBP_SettingsWidget.WBP_SettingsWidget_C"));
}

UToroDatabase* UToroSettings::GetDatabase(const TSubclassOf<UToroDatabase> Class) const
{
	if (Class)
	{
		for (const TSoftObjectPtr<UToroDatabase>& Database : Databases)
		{
			UToroDatabase* DB = Database.LoadSynchronous();
			if (DB && DB->IsA(Class))
			{
				return DB;
			}
		}
	}
	return nullptr;
}
