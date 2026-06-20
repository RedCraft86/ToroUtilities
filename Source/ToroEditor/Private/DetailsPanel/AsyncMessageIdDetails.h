// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "IDetailPropertyRow.h"
#include "DetailsPanel/ToroStructCustomization.h"

#include "AsyncMessageId.h"

// NOTE: This adds customization to a built-in plugin's class and can break
class FAsyncMessageIdDetails final : public FToroStructCustomization
{
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		const TSharedPtr<IPropertyHandle> Property = StructHandle->GetChildHandle(TEXT("InternalMessageTag"));
		Property->MarkHiddenByCustomization();

		if (const FProperty* StructProperty = StructHandle->GetProperty())
		{ 
			if (const TMap<FName, FString>* MetadataMap = StructProperty->GetMetaDataMap())
			{
				for (const TPair<FName, FString>& Metadata : *MetadataMap)
				{
					Property->SetInstanceMetaData(Metadata.Key, Metadata.Value);
				}
			}
		}

		StructBuilder.AddProperty(Property.ToSharedRef())
			.DisplayName(StructHandle->GetPropertyDisplayName())
			.ToolTip(StructHandle->GetToolTipText());
	}
};