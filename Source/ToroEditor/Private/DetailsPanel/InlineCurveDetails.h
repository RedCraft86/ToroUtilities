// Copyright (C) RedCraft86. Licensed under LGPL-3.0, see LICENSE file for details.

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
		FToroStructCustomization::CustomizeChildren(StructHandle, StructBuilder, StructCustomizationUtils);

		GET_STRUCT_PROPERTY_VAR_NS(Curve, Curve);

		ForwardMetadata(Curve);

		StructBuilder.AddProperty(Curve.ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName())
			.ToolTip(StructHandle->GetToolTipText());
	}
};
#undef STRUCT_NAME