// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "IPropertyTypeCustomization.h"

#define GET_STRUCT_PROPERTY(Struct, Member) StructHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(Struct, Member))
#define GET_STRUCT_PROPERTY_NS(Member) GET_STRUCT_PROPERTY(STRUCT_NAME, Member)

#define GET_STRUCT_PROPERTY_VAR(Struct, Member, VarName) \
	TSharedPtr<IPropertyHandle> VarName = GET_STRUCT_PROPERTY(Struct, Member); \
	VarName->MarkHiddenByCustomization();

#define GET_STRUCT_PROPERTY_VAR_NS(Member, VarName) \
	GET_STRUCT_PROPERTY_VAR(STRUCT_NAME, Member, VarName)

class TOROEDITOR_API FToroStructCustomization : public IPropertyTypeCustomization
{
protected:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow, 
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder, 
		IPropertyTypeCustomizationUtils& CustomizationUtils) override;
};
