// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsPanels/CustomizationHelpers.h"
#include "Gameplay/WorldActions/WorldActionBase.h"

class FWorldActionArrayDetails final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FWorldActionArrayDetails>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		StructBuilder.AddProperty(STRUCT_PROPERTY(FWorldActionArray, Actions))
			.DisplayName(StructPropertyHandle->GetPropertyDisplayName());
	}
};