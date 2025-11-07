// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "DetailsPanels/CustomizationHelpers.h"
#include "DataTypes/MathTypes.h"

class FInlineCurveDetails final : public IPropertyTypeCustomization
{
public:

	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShared<FInlineCurveDetails>();
	}

private:

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override {}
	
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		const TSharedRef<IPropertyHandle> Curve = STRUCT_PROPERTY(FInlineFloatCurve, Curve);
		if (const TMap<FName, FString>* Metadata = StructPropertyHandle->GetInstanceMetaDataMap())
		{
			for (const TPair<FName, FString>& Pair : *Metadata)
			{
				Curve->SetInstanceMetaData(Pair.Key, Pair.Value);
			}
		}

		StructBuilder.AddProperty(Curve).DisplayName(StructPropertyHandle->GetPropertyDisplayName());
	}
};