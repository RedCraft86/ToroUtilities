// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

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

class TOROEDITOR_API FToroClassCustomization : public IDetailCustomization
{
protected:

	bool bCustomizeTemplate = false;
	UClass* CustomizingClass = nullptr;
	TWeakPtr<IDetailLayoutBuilder> WeakBuilder;

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	void HideCategory(const FName Category) const;

private:

	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;

	void HandleShowOnlyCategories() const;

public:

	static inline TSet<FString> AlwaysShowCategories{
		TEXT("Transform"), TEXT("TransformCommon")
	};

	static TMap<UClass*, TSet<FString>> DefaultShowOnlyCategories;
};
