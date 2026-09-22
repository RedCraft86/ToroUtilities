// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "ClassViewerFilter.h"

/**
 * Filters asset classes by inheritance, class flags, and Blueprint origin.
 */
class FToroAssetClassFilter : public IClassViewerFilter
{
public:

	bool bDisallowBlueprint;
	EClassFlags DisallowedClassFlags;
	TSet<const UClass*> AllowedBaseClasses;

	FToroAssetClassFilter(const UClass* AllowedClass)
		: bDisallowBlueprint(true), DisallowedClassFlags(CLASS_None), AllowedBaseClasses({ AllowedClass })
	{}

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InitOptions, const UClass* InClass,
		TSharedRef<FClassViewerFilterFuncs> FilterFuncs) override
	{
		if (InClass->HasAnyClassFlags(DisallowedClassFlags) || !InClass->CanCreateAssetOfClass()
			|| FilterFuncs->IfInChildOfClassesSet(AllowedBaseClasses, InClass) == EFilterReturn::Failed)
		{
			return false;
		}

		if (bDisallowBlueprint && Cast<UBlueprint>(InClass->ClassGeneratedBy) != nullptr)
		{
			return false;
		}

		return true;
	}

	virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions,
		const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		if (bDisallowBlueprint)
		{
			return false;
		}

		return !InUnloadedClassData->HasAnyClassFlags(DisallowedClassFlags)
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedBaseClasses, InUnloadedClassData) != EFilterReturn::Failed;
	}
};
