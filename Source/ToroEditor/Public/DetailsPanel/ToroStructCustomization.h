// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "IPropertyTypeCustomization.h"

/**
 * Gets a child property from the active struct property handle.
 */
#define GET_STRUCT_PROPERTY(Struct, Member) \
	StructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(Struct, Member))

/**
 * Gets a child property using the customization's STRUCT_NAME alias.
 */
#define GET_STRUCT_PROPERTY_NS(Member) \
	GET_STRUCT_PROPERTY(STRUCT_NAME, Member)

/**
 * Declares a child property handle and hides its automatically generated row.
 */
#define GET_STRUCT_PROPERTY_VAR(Struct, Member, VarName) \
	TSharedPtr<IPropertyHandle> VarName = GET_STRUCT_PROPERTY(Struct, Member); \
	VarName->MarkHiddenByCustomization();

/**
 * Declares and hides a child property using the customization's STRUCT_NAME alias.
 */
#define GET_STRUCT_PROPERTY_VAR_NS(Member, VarName) \
	GET_STRUCT_PROPERTY_VAR(STRUCT_NAME, Member, VarName)

class TOROEDITOR_API FToroStructCustomization : public IPropertyTypeCustomization
{
protected:

	/**
	 * Struct property handle currently invoking this customization.
	 */
	TWeakPtr<IPropertyHandle> WeakStructHandle;

	/**
	 * Copies metadata from the containing struct property to a child property.
	 * @param Property Child property that receives the metadata.
	 */
	void ForwardMetadata(const TSharedPtr<IPropertyHandle>& Property) const;

	/**
	 * Stores the active struct handle while leaving the default header unchanged.
	 * @param StructHandle Handle for the struct property.
	 * @param HeaderRow Row used to customize the struct header.
	 * @param CustomizationUtils Property customization utilities.
	 */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	/**
	 * Stores the active struct handle while leaving the default children unchanged.
	 * @param StructHandle Handle for the struct property.
	 * @param StructBuilder Builder used to customize child rows.
	 * @param CustomizationUtils Property customization utilities.
	 */
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};
