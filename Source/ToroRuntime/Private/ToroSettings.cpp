// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "ToroSettings.h"
#include "GeneralProjectSettings.h"
#include "Misc/App.h"

UToroSettings::UToroSettings()
{
	CategoryName = TEXT("Project");
	SectionName = TEXT("ToroUtilities");
}

FString UToroSettings::GetVersionString() const
{
	FString Result;
	if (const UGeneralProjectSettings* ProjectSettings = GetDefault<UGeneralProjectSettings>())
	{
		Result.Appendf(TEXT("Version: %s-%s"), *ProjectSettings->ProjectVersion,
			*FString(LexToString(FApp::GetBuildConfiguration())).ToUpper());
	}
	if (!DemoName.IsNone())
	{
		Result.Appendf(TEXT(" | %s"), *DemoName.ToString());
	}
	return Result;
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
