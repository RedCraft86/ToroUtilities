// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"

#include "WorldActions/WorldActionList.h"

#define STRUCT_NAME FWorldActionList
class FWorldActionListDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		GET_STRUCT_PROPERTY_VAR_NS(Actions, Actions);
		if (const TMap<FName, FString>* Metadata = StructHandle->GetInstanceMetaDataMap())
		{
			for (const TPair<FName, FString>& Pair : *Metadata)
			{
				Actions->SetInstanceMetaData(Pair.Key, Pair.Value);
			}
		}

		StructBuilder.AddProperty(Actions.ToSharedRef()).DisplayName(StructHandle->GetPropertyDisplayName());
	}
};
#undef STRUCT_NAME