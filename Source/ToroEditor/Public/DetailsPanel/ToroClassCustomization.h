// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailLayoutBuilder.h"
#include "EditorCategoryUtils.h"
#include "IDetailCustomization.h"
#include "PropertyCustomizationHelpers.h"

#define GET_CLASS_PROPERTY(Class, Member) \
	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(Class, Member))

#define GET_CLASS_PROPERTY_NS(Member) \
	GET_CLASS_PROPERTY(CLASS_NAME, Member)

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

protected:

	bool bCustomizeTemplate = false;
	UClass* CustomizingClass = nullptr;
	TWeakPtr<IDetailLayoutBuilder> WeakBuilder;

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:

	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;
};