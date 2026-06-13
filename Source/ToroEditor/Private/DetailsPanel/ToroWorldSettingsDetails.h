// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "DetailsPanel/ToroClassCustomization.h"
#include "Framework/ToroWorldSettings.h"

#define CLASS_NAME AToroWorldSettings
class FToroWorldSettingsDetails final : public FToroClassCustomization
{
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		// Category ordering
		static const TArray<FName> AllowedCategories = {
			TEXT("PostProcessVolume"),
			TEXT("LightProbes"),
			TEXT("WorldMusic"),
			TEXT("GameMode"),
			TEXT("World"),
			TEXT("Physics"),
			TEXT("AI"),
			TEXT("Tick"),
			TEXT("Navigation"),
			TEXT("Rendering"),
			TEXT("Audio"),
			TEXT("WorldPartitionSetup"),
			TEXT("Foliage"),
			TEXT("HLODSystem")
		};
		for (const FName& CategoryName : AllowedCategories)
		{
			SetCategoryPriority(CategoryName, ECategoryPriority::Default);
		}

		TArray<FName> AllCategories;
		DetailBuilder.GetCategoryNames(AllCategories);
		for (const FName& CategoryName : AllCategories)
		{
			bool bAllowed = false;
			for (const FName& AllowedCategory : AllowedCategories)
			{
				if (CategoryName.ToString().StartsWith(AllowedCategory.ToString()))
				{
					bAllowed = true;
					break;
				}
			}

			if (!bAllowed)
			{
				DetailBuilder.HideCategory(CategoryName);
			}
		}

		// Hide properties from post process
		GET_CLASS_PROPERTY_VAR_NS(PostProcess, PostProcess)
		static const TArray<FName> HiddenPostProcessProperties = {
			TEXT("Priority"),
			TEXT("BlendRadius"),
			TEXT("BlendWeight"),
			TEXT("bUnbound"),
			TEXT("bEnabled")
		};
		for (const FName& PropertyName : HiddenPostProcessProperties)
		{
			TSharedPtr<IPropertyHandle> ChildProp = PostProcess->GetChildHandle(PropertyName);
			if (ChildProp.IsValid())
			{
				DetailBuilder.HideProperty(ChildProp);
			}
		}
	}
};
#undef STRUCT_NAME