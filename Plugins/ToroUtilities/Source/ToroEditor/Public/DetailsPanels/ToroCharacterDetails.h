// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "DetailLayoutBuilder.h"
#include "EditorCategoryUtils.h"
#include "IDetailCustomization.h"
#include "Actors/ToroCharacter.h"

#define CLASSNAME AToroCharacter
class TOROEDITOR_API FToroCharacterDetails : public IDetailCustomization
{
public:

	static inline TSet<FName> AlwaysShowCategories = {
		"Transform", "TransformCommon", "Actor"
	};

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShared<FToroCharacterDetails>();
	}

protected:

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		PreApplyCustomization(DetailBuilder);

		const UClass* TargetClass = nullptr;
		TArray<TWeakObjectPtr<UObject>> Objects;
		DetailBuilder.GetObjectsBeingCustomized(Objects);
		for (const TWeakObjectPtr<UObject>& Object : Objects)
		{
			if (!Object.IsValid()) continue;

			// Exit on CDO (Blueprint view)
			if (Object->IsTemplate()) return;

			// Exit on different classes
			if (!TargetClass) TargetClass = Object->GetClass();
			else if (TargetClass != Object->GetClass()) return;
		}

		OnCustomizationValid(DetailBuilder);

		TArray<FString> Priority, Show, Hide;
		TargetClass->GetPrioritizeCategories(Priority);
		FEditorCategoryUtils::GetClassShowCategories(TargetClass, Show);
		FEditorCategoryUtils::GetClassHideCategories(TargetClass, Hide);
		Show.RemoveAll([&Hide](const FString& Category)
		{
			return Hide.Contains(Category);
		});

		TArray<FName> CategoryNames;
		DetailBuilder.GetCategoryNames(CategoryNames);
		CategoryNames.RemoveAll([&Show](const FName& Category)
		{
			return AlwaysShowCategories.Contains(Category)
				|| Show.Contains(Category.ToString())
				|| Category.ToString().Contains(TEXT("|")); // Skip SubCategories
		});

		for (const FName& CategoryName : CategoryNames)
		{
			const FString Category = CategoryName.ToString();
			if (Priority.Contains(CategoryName) && !Hide.Contains(CategoryName))
			{
				DetailBuilder.EditCategory(CategoryName,
					FText::GetEmpty(),
					ECategoryPriority::Important);
			}
			else
			{
				DetailBuilder.HideCategory(CategoryName);
			}
		}

		PostApplyCustomization(DetailBuilder);
	}

	virtual void OnCustomizationValid(IDetailLayoutBuilder& DetailBuilder) {}
	virtual void PreApplyCustomization(IDetailLayoutBuilder& DetailBuilder) {}
	virtual void PostApplyCustomization(IDetailLayoutBuilder& DetailBuilder) {}
};
#undef CLASSNAME