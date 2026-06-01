// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"

#include "DataTypes/InlineCurves.h"

#define STRUCT_NAME FInlineFloatCurve
class FInlineCurveDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		GET_STRUCT_PROPERTY_VAR_NS(Curve, Curve);
		if (const TMap<FName, FString>* Metadata = StructHandle->GetInstanceMetaDataMap())
		{
			for (const TPair<FName, FString>& Pair : *Metadata)
			{
				Curve->SetInstanceMetaData(Pair.Key, Pair.Value);
			}
		}

		StructBuilder.AddProperty(Curve.ToSharedRef()).DisplayName(StructHandle->GetPropertyDisplayName());
	}
};
#undef STRUCT_NAME