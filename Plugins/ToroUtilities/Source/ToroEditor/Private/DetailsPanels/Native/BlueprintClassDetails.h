// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailGroup.h"
#include "DetailLayoutBuilder.h"
#include "IDetailCustomization.h"
#include "DetailsPanels/CustomizationHelpers.h"

#define LOCTEXT_NAMESPACE "ToroEditor"
class FBlueprintClassDetails final : public IDetailCustomization
{
public:

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FBlueprintClassDetails>();
	}

protected:

	static inline FName NAME_PrioritizeCategories = "PrioritizeCategories";

	TWeakObjectPtr<UBlueprint> BlueprintPtr;

	TArray<FString> GetPrioritizeCategories() const
	{
		TArray<FString> Result;
		if (BlueprintPtr.IsValid() && BlueprintPtr->GeneratedClass)
		{
			BlueprintPtr->GeneratedClass->GetPrioritizeCategories(Result);
		}
		return Result;
	}

	void AddPrioritizeCategory(const FString& Category) const
	{
		if (!BlueprintPtr.IsValid() || !BlueprintPtr->GeneratedClass) return;
		if (FString* Categories = BlueprintPtr->GeneratedClass->FindMetaData(NAME_PrioritizeCategories))
		{
			Categories->ReplaceInline(*Category, TEXT("")); // Remove actual entry
			Categories->ReplaceInline(TEXT("  "), TEXT(" ")); // Replace double space with single
			Categories->TrimStartAndEndInline(); // Trim start and end from any leading or trailing spaces
		}
	}

	void RemovePrioritizeCategory(const FString& Category) const
	{
		if (!BlueprintPtr.IsValid() || !BlueprintPtr->GeneratedClass) return;
		if (FString* Categories = BlueprintPtr->GeneratedClass->FindMetaData(NAME_PrioritizeCategories))
		{
			Categories->Appendf(TEXT(" %s"), *Category);
		}
	}

	TArray<FString> PrioritizeCategories = GetPrioritizeCategories();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		TArray<TWeakObjectPtr<UObject>> Objects;
		DetailBuilder.GetObjectsBeingCustomized(Objects);
		if (Objects.Num() != 1)
		{
			return;
		}

		BlueprintPtr = Cast<UBlueprint>(Objects[0]);
		if (!BlueprintPtr.IsValid())
		{
			return;
		}

		IDetailCategoryBuilder& OptionCategory = DetailBuilder.EditCategory("BlueprintOptions");
		IDetailGroup& PrioritizeCategoriesGroup = OptionCategory.AddGroup("PrioritizeCategories",
			LOCTEXT("PrioritizeCategoriesName", "Prioritize Categories"));
		
		PrioritizeCategoriesGroup.HeaderRow()
		.NameContent()
		[
			SNew(STextBlock)
			.ToolTip(FSlateApplicationBase::Get().MakeToolTip(LOCTEXT("PrioritizeCategoriesTooltip", "Categories to prioritize showing")))
			.Text(LOCTEXT("PrioritizeCategoriesName", "Prioritize Categories"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::Printf(TEXT("%d Entries"), PrioritizeCategories.Num())))
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
			// +SHorizontalBox::Slot()
			// .AutoWidth()
			// .Padding(2.0f, 0.0f, 0.0f, 0.0f)
			// [
			// 	PropertyCustomizationHelpers::MakeAddButton(FSimpleDelegate::CreateLambda(OnAddMetadata), LOCTEXT("MetadataAddName", "Add Metadata"))
			// ]
		];
	}
};
#undef LOCTEXT_NAMESPACE