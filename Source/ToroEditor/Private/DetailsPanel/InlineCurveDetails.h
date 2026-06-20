// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

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
		if (const FProperty* StructProperty = StructHandle->GetProperty())
		{ 
			if (const TMap<FName, FString>* MetadataMap = StructProperty->GetMetaDataMap())
			{
				for (const TPair<FName, FString>& Metadata : *MetadataMap)
				{
					Curve->SetInstanceMetaData(Metadata.Key, Metadata.Value);
				}
			}
		}

		StructBuilder.AddProperty(Curve.ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName())
			.ToolTip(StructHandle->GetToolTipText());
	}
};
#undef STRUCT_NAME