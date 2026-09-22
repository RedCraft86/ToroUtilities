// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "DetailLayoutBuilder.h"
#include "EditorCategoryUtils.h"
#include "IDetailCustomization.h"
#include "ToroCustomizationHelpers.h"
#include "PropertyCustomizationHelpers.h"

/**
 * Gets a reflected class property from the active Details layout.
 */
#define GET_CLASS_PROPERTY(Class, Member) \
	DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(Class, Member))

/**
 * Gets a property using the customization's CLASS_NAME alias.
 */
#define GET_CLASS_PROPERTY_NS(Member) \
	GET_CLASS_PROPERTY(CLASS_NAME, Member)

/**
 * Declares a property handle and hides its automatically generated row.
 */
#define GET_CLASS_PROPERTY_VAR(Class, Member, VarName) \
	TSharedRef<IPropertyHandle> VarName = GET_CLASS_PROPERTY(Class, Member); \
	VarName->MarkHiddenByCustomization();

/**
 * Declares and hides a property using the customization's CLASS_NAME alias.
 */
#define GET_CLASS_PROPERTY_VAR_NS(Member, VarName) \
	GET_CLASS_PROPERTY_VAR(CLASS_NAME, Member, VarName)

/**
 * Class customization with category filtering, ordering, and renaming support.
 * <c>GetAlwaysShowCategories()</c> bypasses filtering, while <c>GetShowOnlyCategories()</c> limits the remaining categories.
 * <c>HideCategories</c> supports <c>*</c> to hide all categories not explicitly allowed.
 * <c>RenameCategories</c> accepts comma-separated mappings such as <c>Category1=First One</c>.
 */
class TOROEDITOR_API FToroClassCustomization : public IDetailCustomization
{
protected:

	/**
	 * Display overrides retained for a category while customization is active.
	 */
	struct FToroCategoryInfo
	{
		FText DisplayName;
		ECategoryPriority::Type Priority;

		FToroCategoryInfo()
			: DisplayName(FText::GetEmpty()), Priority(ECategoryPriority::Default)
		{}
	};

	/**
	 * Whether class-default and other template objects may be customized.
	 */
	bool bCustomizeTemplate = false;

	/**
	 * Common concrete class of the objects currently being customized.
	 */
	UClass* CustomizingClass = nullptr;

	/**
	 * Display overrides recorded for edited categories.
	 */
	TMap<FName, FToroCategoryInfo> CategoryMap = {};

	/**
	 * Details layout currently invoking this customization.
	 */
	TWeakPtr<IDetailLayoutBuilder> WeakBuilder;

	/**
	 * Finds or creates a category using its recorded display overrides.
	 * @param Category Internal category name.
	 */
	IDetailCategoryBuilder& FindOrAddCategory(const FName Category) const;

	/**
	 * Changes a category's displayed name.
	 * @param Category Internal category name.
	 * @param DisplayName New localized display name.
	 */
	IDetailCategoryBuilder& SetCategoryDisplayName(const FName Category, const FText& DisplayName);

	/**
	 * Changes a category's sorting priority.
	 * @param Category Internal category name.
	 * @param Priority New category priority.
	 */
	IDetailCategoryBuilder& SetCategoryPriority(const FName Category, const ECategoryPriority::Type Priority);

	/**
	 * Gets categories that remain visible regardless of other filtering.
	 */
	virtual TArray<FString> GetAlwaysShowCategories();

	/**
	 * Gets the category allowlist used when show-only filtering is enabled.
	 * <c>HideCategories</c> can still exclude entries returned here.
	 */
	virtual TArray<FString> GetShowOnlyCategories();

	/**
	 * Adds customization-specific rows after category processing is complete.
	 * @param DetailBuilder Active Details layout builder.
	 */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:

	/**
	 * Prepares category filtering before forwarding to the reference overload.
	 * @param DetailBuilder Shared Details layout builder supplied by the Property Editor.
	 */
	virtual void CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder) override;
};
