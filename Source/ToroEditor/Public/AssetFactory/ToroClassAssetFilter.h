// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

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
