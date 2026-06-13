// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "DetailLayoutBuilder.h"
#include "EditorCategoryUtils.h"
#include "IDetailCustomization.h"
#include "PropertyCustomizationHelpers.h"

#define GET_CLASS_PROPERTY(Class, Member) DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(Class, Member))
#define GET_CLASS_PROPERTY_NS(Member) GET_CLASS_PROPERTY(CLASS_NAME, Member)

#define GET_CLASS_PROPERTY_VAR(Class, Member, VarName) \
	TSharedRef<IPropertyHandle> VarName = GET_CLASS_PROPERTY(Class, Member); \
	VarName->MarkHiddenByCustomization();

#define GET_CLASS_PROPERTY_VAR_NS(Member, VarName) \
	GET_CLASS_PROPERTY_VAR(CLASS_NAME, Member, VarName)

struct FToroCategoryInfo
{
	FText DisplayName;
	ECategoryPriority::Type Priority;

	FToroCategoryInfo()
		: DisplayName(FText::GetEmpty()), Priority(ECategoryPriority::Default)
	{}
};

class TOROEDITOR_API FToroClassCustomization : public IDetailCustomization
{
public:

	static inline TSet<FString> DefaultShowCategories{
		TEXT("Transform"), TEXT("TransformCommon")
	};

	static TMap<UClass*, TSet<FString>> DefaultShowOnlyCategories;

protected:

	bool bCustomizeTemplate = false;
	UClass* CustomizingClass = nullptr;
	TWeakPtr<IDetailLayoutBuilder> WeakBuilder;

	TArray<FString> ClassShowCategories;
	TArray<FString> ClassHideCategories;
	TArray<FString> ClassPrioritizeCategories;
	TMap<FName, FToroCategoryInfo> CategoryMap;

	IDetailCategoryBuilder& FindOrAddCategory(const FName CategoryName) const;
	IDetailCategoryBuilder& SetCategoryDisplayName(const FName CategoryName, const FText& DisplayName);
	IDetailCategoryBuilder& SetCategoryPriority(const FName CategoryName, const ECategoryPriority::Type Priority);

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:

	void HandleCategoryRenames();
	void HandleCategoryPriority();
	void HandleShowOnlyCategories() const;
	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;
};
