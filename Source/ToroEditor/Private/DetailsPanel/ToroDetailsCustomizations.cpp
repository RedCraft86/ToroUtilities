// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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

IDetailCategoryBuilder& FToroClassCustomization::FindOrAddCategory(const FName CategoryName) const
{
	if (const FToroCategoryInfo* Info = CategoryMap.Find(CategoryName))
	{
		return WeakBuilder.Pin()->EditCategory(CategoryName, Info->DisplayName, Info->Priority);
	}
	return WeakBuilder.Pin()->EditCategory(CategoryName);
}

IDetailCategoryBuilder& FToroClassCustomization::SetCategoryDisplayName(const FName CategoryName, const FText& DisplayName)
{
	CategoryMap.FindOrAdd(CategoryName).DisplayName = DisplayName;
	return FindOrAddCategory(CategoryName);
}

IDetailCategoryBuilder& FToroClassCustomization::SetCategoryPriority(const FName CategoryName, const ECategoryPriority::Type Priority)
{
	CategoryMap.FindOrAdd(CategoryName).Priority = Priority;
	return FindOrAddCategory(CategoryName);
}

void FToroClassCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {}

void FToroClassCustomization::HandleCategoryRenames()
{
	if (!WeakBuilder.IsValid())
	{
		return;
	}

	static const FName NAME_RenameCategories("RenameCategories");
	const FString& MetaValue = CustomizingClass->GetMetaData(NAME_RenameCategories);

	TArray<FString> Renames;
	MetaValue.ParseIntoArray(Renames, TEXT(","));
	for (const FString& Rename : Renames)
	{
		FString From, To;
		Rename.Split(TEXT("="), &From, &To, ESearchCase::IgnoreCase);
		From.TrimStartAndEndInline();
		To.TrimStartAndEndInline();

		SetCategoryDisplayName(*From, FText::FromString(To));
	}
}

void FToroClassCustomization::HandleCategoryPriority()
{
	if (!WeakBuilder.IsValid())
	{
		return;
	}

	if (ClassPrioritizeCategories.IsEmpty())
	{
		SetCategoryPriority(TEXT("Settings"), ECategoryPriority::Important);
		SetCategoryPriority(TEXT("Tools"), ECategoryPriority::Important);
	}
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

	// There must be a valid DefaultShowOnly entry OR there are HideCategories present with the first item being "*"
	if (InvolvedParent != nullptr || (!ClassHideCategories.IsEmpty() && ClassHideCategories[0] == TEXT("*")))
	{
		TSet<FString> AllowedCategories = DefaultShowCategories;
		AllowedCategories.Append(DefaultShowOnlyCategories.FindRef(InvolvedParent));
		AllowedCategories.Append(ClassShowCategories);

		TArray<FName> CategoriesToHide;
		WeakBuilder.Pin()->GetCategoryNames(CategoriesToHide);
		CategoriesToHide.RemoveAll([this, &AllowedCategories](const FName& Element)
		{
			const FString Category = Element.ToString();
			for (const FString& HideCat : ClassHideCategories)
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
			WeakBuilder.Pin()->HideCategory(Name);
		}
	}
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

	FEditorCategoryUtils::GetClassHideCategories(CustomizingClass, ClassHideCategories);
	FEditorCategoryUtils::GetClassShowCategories(CustomizingClass, ClassShowCategories);
	CustomizingClass->GetPrioritizeCategories(ClassPrioritizeCategories);

	HandleShowOnlyCategories();
	HandleCategoryPriority();
	HandleCategoryRenames();

	IDetailCustomization::CustomizeDetails(DetailBuilder);
}

void FToroStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{}

void FToroStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle,
	IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{}