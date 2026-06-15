// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ClassViewerFilter.h"
#include "EdGraphSchema_K2.h"

class TOROEDITOR_API FToroClassAssetFilter final : public IClassViewerFilter
{
public:

	FToroClassAssetFilter(UClass* ForClass, const bool bDisallowBlueprint, const EClassFlags DisallowedFlags)
		: bSkipBlueprints(bDisallowBlueprint)
		, DisallowedClassFlags(DisallowedFlags)
		, AllowedParents({ForClass})
	{}

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, 
		const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !InClass->HasAnyClassFlags(DisallowedClassFlags)
			&& (!bSkipBlueprints || !InClass->GetBoolMetaDataHierarchical(FBlueprintMetadata::MD_IsBlueprintBase))
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedParents, InClass) != EFilterReturn::Failed;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, 
		const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, 
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		return !bSkipBlueprints && !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedParents, InUnloadedClassData) != EFilterReturn::Failed;
	}

private:

	bool bSkipBlueprints;
	EClassFlags DisallowedClassFlags;
	TSet<const UClass*> AllowedParents;
};
