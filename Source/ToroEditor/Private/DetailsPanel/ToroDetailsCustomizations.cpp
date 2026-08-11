// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

// ReSharper disable CppMemberFunctionMayBeStatic
#include "DetailsPanel/ToroClassCustomization.h"
#include "DetailsPanel/ToroStructCustomization.h"

IDetailCategoryBuilder& FToroClassCustomization::FindOrAddCategory(const FName Category) const
{
	if (const FToroCategoryInfo* Info = CategoryMap.Find(Category))
	{
		return WeakBuilder.Pin()->EditCategory(Category, Info->DisplayName, Info->Priority);
	}

	return WeakBuilder.Pin()->EditCategory(Category);
}

IDetailCategoryBuilder& FToroClassCustomization::SetCategoryDisplayName(const FName Category, const FText& DisplayName)
{
	CategoryMap.FindOrAdd(Category).DisplayName = DisplayName;
	return FindOrAddCategory(Category);
}

IDetailCategoryBuilder& FToroClassCustomization::SetCategoryPriority(const FName Category, const ECategoryPriority::Type Priority)
{
	CategoryMap.FindOrAdd(Category).Priority = Priority;
	return FindOrAddCategory(Category);
}

TArray<FString> FToroClassCustomization::GetAlwaysShowCategories()
{
	return TArray<FString>{
		TEXT("Transform"), TEXT("TransformCommon")
	};
}

TArray<FString> FToroClassCustomization::GetShowOnlyCategories()
{
	return TArray<FString>();
}

void FToroClassCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {}

void FToroClassCustomization::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	WeakBuilder = DetailBuilder;

	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder->GetObjectsBeingCustomized(Objects);
	for (const TWeakObjectPtr<UObject>& Object : Objects)
	{
		// Check Validity        Check CDO / Blueprint View
		if (!Object.IsValid() || (!bCustomizeTemplate && Object->IsTemplate()))
		{
			return;
		}

		if (!CustomizingClass)
		{
			CustomizingClass = Object->GetClass();
		}
		else if (CustomizingClass != Object->GetClass())
		{
			// Disallow customizing on different classes
			return;
		}
	}

	if (!CustomizingClass)
	{
		return;
	}

#pragma region ClassCategory

	auto ArrayHasCategory = [](const TArray<FString>& Array, const FString& Category)
	{
		for (const FString& Entry : Array)
		{
			// Actual category is shorter than what we're checking against, skip.
			if (Category.Len() < Entry.Len())
			{
				continue;
			}
			if (Category.Len() == Entry.Len())
			{
				// Actual category length matches, it is true only if the strings match as well
				if (Category == Entry)
				{
					return true;
				}
			}
			else if (Category.StartsWith(Entry) && Category[Entry.Len()] == TEXT('|'))
			{
				// Actual category is longer, it is true only if it's a subcategory. The next letter needs to be a '|'
				return true;
			}
		}

		return false;
	};

	TArray<FString> ClassHideCategories, ClassShowCategories, ClassPrioritizeCategories;
	FEditorCategoryUtils::GetClassHideCategories(CustomizingClass, ClassHideCategories);
	FEditorCategoryUtils::GetClassShowCategories(CustomizingClass, ClassShowCategories);
	CustomizingClass->GetPrioritizeCategories(ClassPrioritizeCategories);

	bool bHideAllNativeCategories = false;
	if (!ClassHideCategories.IsEmpty() && ClassHideCategories[0] == FString(TEXT("*")))
	{
		bHideAllNativeCategories = true;
		ClassHideCategories.RemoveAtSwap(0);
	}

	TArray<FString> AllowedCategories = GetAlwaysShowCategories();
	AllowedCategories.Append(ClassPrioritizeCategories);
	AllowedCategories.Append(ClassShowCategories);

	const TArray<FString> ShowOnlyCategories = GetShowOnlyCategories();
	if (!ShowOnlyCategories.IsEmpty())
	{
		bHideAllNativeCategories = true;
		if (ClassHideCategories.IsEmpty())
		{
			AllowedCategories.Append(ShowOnlyCategories);
		}
		else
		{
			AllowedCategories.Append(ShowOnlyCategories.FilterByPredicate([&](const FString& Entry)
			{
				return !ArrayHasCategory(ClassHideCategories, Entry);
			}));
		}
	}

	TArray<FName> HidingCategories;
	WeakBuilder.Pin()->GetCategoryNames(HidingCategories);
	HidingCategories.RemoveAllSwap([&](const FName& Element)
	{
		const FString Category = Element.ToString();
		if (ArrayHasCategory(AllowedCategories, Category))
		{
			return true;
		}

		if (ArrayHasCategory(ClassHideCategories, Category))
		{
			return false;
		}

		return !bHideAllNativeCategories;
	});

	for (const FName& Category : HidingCategories)
	{
		WeakBuilder.Pin()->HideCategory(Category);
	}

	if (ClassPrioritizeCategories.IsEmpty())
	{
		SetCategoryPriority(TEXT("Settings"), ECategoryPriority::Important);
		SetCategoryPriority(TEXT("Tools"), ECategoryPriority::Important);
	}

	TMap<FString, FText> ClassRenameCategories;
	ToroCustomizationHelpers::GetRenameCategories(CustomizingClass, ClassRenameCategories);
	for (const TPair<FString, FText>& Rename : ClassRenameCategories)
	{
		SetCategoryDisplayName(*Rename.Key, Rename.Value);
	}
#pragma endregion

	IDetailCustomization::CustomizeDetails(DetailBuilder);
}

void FToroStructCustomization::ForwardMetadata(const TSharedPtr<IPropertyHandle>& Property) const
{
	if (WeakStructHandle.IsValid() && Property.IsValid())
	{
		if (const FProperty* StructProperty = WeakStructHandle.Pin()->GetProperty())
		{
			if (const TMap<FName, FString>* MetadataMap = StructProperty->GetMetaDataMap())
			{
				for (const TPair<FName, FString>& Metadata : *MetadataMap)
				{
					Property->SetInstanceMetaData(Metadata.Key, Metadata.Value);
				}
			}
		}
	}
}

void FToroStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	WeakStructHandle = StructHandle;
}

void FToroStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle,
	IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	WeakStructHandle = StructHandle;
}