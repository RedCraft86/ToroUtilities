// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "ClassViewerFilter.h"
#include "Kismet2/KismetEditorUtilities.h"

/**
 * Generic class for the class filter used in ToroAssetFactory.
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

	virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass,
		TSharedRef<FClassViewerFilterFuncs> InFilterFuncs) override
	{
		const bool bAllowed = !InClass->HasAnyClassFlags(DisallowedClassFlags) && InClass->CanCreateAssetOfClass()
			&& InFilterFuncs->IfInChildOfClassesSet(AllowedBaseClasses, InClass) != EFilterReturn::Failed;

		if (bAllowed && bDisallowBlueprint)
		{
			if (FKismetEditorUtilities::CanCreateBlueprintOfClass(InClass))
			{
				return false;
			}
		}

		return bAllowed;
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
