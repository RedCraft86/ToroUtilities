// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "DetailsPanel/ToroClassCustomization.h"
#include "DetailsPanel/ToroStructCustomization.h"
#include "GameFramework/Character.h"

TMap<UClass*, TSet<FString>> FToroClassCustomization::DefaultShowOnlyCategories = {
	{
		ACharacter::StaticClass(), {
			TEXT("Settings"),
			TEXT("Tools"),
			TEXT("Lighting"),
			TEXT("Rendering"),
			// TEXT("HLOD"),
			// TEXT("Mobile"),
			// TEXT("RayTracing"),
			TEXT("Pawn"),
			// TEXT("Replication"),
			// TEXT("Networking"),
			TEXT("Input"),
			TEXT("Actor"),
			TEXT("Optimization"),
			TEXT("LevelOfDetail"),
			TEXT("MaterialParameters"),
			TEXT("TextureStreaming"),
			TEXT("WorldPartition"),
			TEXT("LevelInstance"),
			TEXT("DataLayers")
		}
	}
};

void FToroClassCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {}

void FToroClassCustomization::HideCategory(const FName Category) const
{
	if (!WeakBuilder.IsValid()) return;
	for (const FString& Name : AlwaysShowCategories)
	{
		if (Category.ToString().StartsWith(Name))
		{
			return;
		}
	}

	WeakBuilder.Pin()->HideCategory(Category);
}

void FToroClassCustomization::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	WeakBuilder = DetailBuilder;

	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder->GetObjectsBeingCustomized(Objects);

	for (const TWeakObjectPtr<UObject>& Object : Objects)
	{
		if (!Object.IsValid()) return; // Check Validity
		if (Object->IsTemplate() && !bCustomizeTemplate) return; // Check CDO / Blueprint View
		if (!CustomizingClass)
		{
			CustomizingClass = Object->GetClass();
		}
		else if (CustomizingClass != Object->GetClass())
		{
			// No customizing on different classes
			return;
		}
	}

	if (!CustomizingClass)
	{
		return;
	}

	HandleShowOnlyCategories();
		
	IDetailCustomization::CustomizeDetails(DetailBuilder);
}

void FToroClassCustomization::HandleShowOnlyCategories() const
{
	if (!WeakBuilder.IsValid())
	{
		return;
	}

	const UClass* InvolvedParent = nullptr;
	for (const TPair<UClass*, TSet<FString>>& Categories : DefaultShowOnlyCategories)
	{
		if (Categories.Key && CustomizingClass->IsChildOf(Categories.Key))
		{
			InvolvedParent = Categories.Key;
			break;
		}
	}

	TArray<FString> HideCategories;
	FEditorCategoryUtils::GetClassHideCategories(CustomizingClass, HideCategories);

	// There must be a valid DefaultShowOnly entry OR there are HideCategories present with the first item being "*"
	if (InvolvedParent != nullptr || (!HideCategories.IsEmpty() && HideCategories[0].Equals("*")))
	{
		TSet<FString> AllowedCategories = DefaultShowOnlyCategories.FindRef(InvolvedParent);

		TArray<FString> ShowCategories;
		FEditorCategoryUtils::GetClassShowCategories(CustomizingClass, ShowCategories);
		AllowedCategories.Append(ShowCategories);

		TArray<FName> CategoriesToHide;
		WeakBuilder.Pin()->GetCategoryNames(CategoriesToHide);
		CategoriesToHide.RemoveAll([&HideCategories, &AllowedCategories](const FName& Element)
		{
			const FString Category = Element.ToString();
			for (const FString& HideCat : HideCategories)
			{
				if (Category.StartsWith(HideCat))
				{
					return false;
				}
			}

			for (const FString& ShowCat : AllowedCategories)
			{
				if (Category.StartsWith(ShowCat))
				{
					return true;
				}
			}

			return false;
		});

		for (const FName& Name : CategoriesToHide)
		{
			HideCategory(Name);
		}
	}
}

void FToroStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{}

void FToroStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle,
	IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{}