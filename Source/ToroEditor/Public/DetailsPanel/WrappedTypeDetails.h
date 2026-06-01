// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"

#include "DataTypes/WrappedTypes.h"

#define STRUCT_NAME FWrappedBool
class FWrappedTypeDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		GET_STRUCT_PROPERTY_VAR_NS(Value, Value);
		if (const TMap<FName, FString>* Metadata = StructHandle->GetInstanceMetaDataMap())
		{
			for (const TPair<FName, FString>& Pair : *Metadata)
			{
				Value->SetInstanceMetaData(Pair.Key, Pair.Value);
			}
		}

		StructBuilder.AddProperty(Value.ToSharedRef()).DisplayName(StructHandle->GetPropertyDisplayName());
	}
};
#undef STRUCT_NAME