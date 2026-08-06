// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "DetailsPanel/ToroClassCustomization.h"
#include "DetailsPanel/ToroStructCustomization.h"

void FToroClassCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {}

void FToroClassCustomization::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	WeakBuilder = DetailBuilder;

	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder->GetObjectsBeingCustomized(Objects);
	for (const TWeakObjectPtr<UObject>& Object : Objects)
	{
		//  Check Validity        Check CDO / Blueprint View
		if (!Object.IsValid() || (!bCustomizeTemplate && Object->IsTemplate()))
		{
			return;
		}

		if (!CustomizingClass)
		{
			CustomizingClass = Object->GetClass();
		}
		else if (CustomizingClass != Object->GetClass())
		{
			// Disallow customizing on different classes
			return;
		}
	}

	if (!CustomizingClass)
	{
		return;
	}

	// TODO: Category madness

	IDetailCustomization::CustomizeDetails(DetailBuilder);
}

void FToroStructCustomization::ForwardMetadata(const TSharedPtr<IPropertyHandle>& Property) const
{
	if (WeakStructHandle.IsValid() && Property.IsValid())
	{
		if (const FProperty* StructProperty = WeakStructHandle.Pin()->GetProperty())
		{ 
			if (const TMap<FName, FString>* MetadataMap = StructProperty->GetMetaDataMap())
			{
				for (const TPair<FName, FString>& Metadata : *MetadataMap)
				{
					Property->SetInstanceMetaData(Metadata.Key, Metadata.Value);
				}
			}
		}
	}
}

void FToroStructCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	WeakStructHandle = StructHandle;
}

void FToroStructCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle,
	IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	WeakStructHandle = StructHandle;
}