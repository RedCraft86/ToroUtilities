// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailLayoutBuilder.h"
#include "EditorCategoryUtils.h"
#include "IDetailCustomization.h"
#include "ToroCustomizationHelpers.h"
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

/**
 * Specialized class customization class with more category controls.
 *
 * <b>Category Systems:</b>
 * - <c>GetAlwaysShowCategories()</c> will show the returned categories regardless of other modifications.
 * - <c>GetShowOnlyCategories()</c> will only show categories that are returned + provided in 
 * <c>GetAlwaysShowCategories()</c>, <c>ShowCategories</c>, <c>PrioritizeCategories</c>, and <c>RenameCategories</c>.\n
 * <c>HideCategories</c> can still be used to hide parts of the returned category as a class-level gate.
 * - <c>*</c> can be added to <c>HideCategories</c> and works the same way as <c>GetAlwaysShowCategories()</c>
 * but without a default list of categories to show as this is a class-level modification.
 * - <c>RenameCategories</c> is a new metadata that can be used to set the display name of categories.\n
 * e.g. <c>Category1 = First One, Category2 = Another One</c>.
 */
class TOROEDITOR_API FToroClassCustomization : public IDetailCustomization
{
protected:

	struct FToroCategoryInfo
	{
		FText DisplayName;
		ECategoryPriority::Type Priority;

		FToroCategoryInfo()
			: DisplayName(FText::GetEmpty()), Priority(ECategoryPriority::Default)
		{}
	};

	bool bCustomizeTemplate = false;
	UClass* CustomizingClass = nullptr;
	TMap<FName, FToroCategoryInfo> CategoryMap = {};
	TWeakPtr<IDetailLayoutBuilder> WeakBuilder;

	IDetailCategoryBuilder& FindOrAddCategory(const FName Category) const;
	IDetailCategoryBuilder& SetCategoryDisplayName(const FName Category, const FText& DisplayName);
	IDetailCategoryBuilder& SetCategoryPriority(const FName Category, const ECategoryPriority::Type Priority);

	/**
	 * Will show the returned categories regardless of other modifications.
	 */
	virtual TArray<FString> GetAlwaysShowCategories();

	/**
	 * If provided, will only show categories that are returned + provided in 
	 * <c>GetAlwaysShowCategories()</c>, <c>ShowCategories</c>, <c>PrioritizeCategories</c>, and <c>RenameCategories</c>.
	 *
	 * <c>HideCategories</c> can still be used to hide parts of the returned category as a class-level gate.
	 */
	virtual TArray<FString> GetShowOnlyCategories();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:

	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;
};