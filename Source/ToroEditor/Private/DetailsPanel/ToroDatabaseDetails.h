// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "SGameplayTagCombo.h"
#include "DetailsPanel/ToroClassCustomization.h"
#include "DataAssets/ToroDatabase.h"

#define CLASS_NAME UToroDatabase
class FToroDatabaseDetails final : public FToroClassCustomization
{
	friend class FToroDatabaseKeyDetails;

	static inline const FName NAME_TagFilter = TEXT("TagFilter");
	static inline const FName NAME_BaseStruct = TEXT("BaseStruct");

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override
	{
		FString FilterTag = FString();
		{
			const TSharedRef<IPropertyHandle> RootTag = GET_CLASS_PROPERTY(CLASS_NAME, RootTag);
			const TSharedPtr<IPropertyHandle> NameTag = RootTag->GetChildHandle(0);
			FName ValueName = NAME_None;
			NameTag->GetValue(ValueName);
			if (!ValueName.IsNone())
			{
				FilterTag = ValueName.ToString().TrimStartAndEnd();
			}
		}

		FString FilterStruct = FString();
		{
			const TSharedRef<IPropertyHandle> RootStruct = GET_CLASS_PROPERTY(CLASS_NAME, RootStruct);
			UObject* ValuePtr = nullptr;
			RootStruct->GetValue(ValuePtr);
			if (const UScriptStruct* StructPtr = Cast<UScriptStruct>(ValuePtr))
			{
				FilterStruct = StructPtr->GetPathName().TrimStartAndEnd();
			}
		}

		const TSharedRef<IPropertyHandle> Entries = GET_CLASS_PROPERTY(CLASS_NAME, Entries);
		const TSharedPtr<IPropertyHandleMap> EntriesMap = Entries->AsMap();

		EntriesMap->SetOnNumElementsChanged(FSimpleDelegate::CreateLambda([&DetailBuilder]()
		{
			DetailBuilder.ForceRefreshDetails();
		}));

		uint32 NumElements;
		EntriesMap->GetNumElements(NumElements);
		for (uint32 i = 0; i < NumElements; i++)
		{
			const TSharedRef<IPropertyHandle> Entry = EntriesMap->GetElement(i);
			if (Entry->IsValidHandle())
			{
				if (!FilterStruct.IsEmpty())
				{
					Entry->SetInstanceMetaData(NAME_BaseStruct, FilterStruct);
				}

				const TSharedPtr<IPropertyHandle> Key = Entry->GetKeyHandle();				
				if (Key && !FilterTag.IsEmpty())
				{
					Key->SetInstanceMetaData(NAME_TagFilter, FilterTag);
				}
			}
		}
	}
};
#undef CLASS_NAME

#define STRUCT_NAME FToroDatabaseKey
class FToroDatabaseKeyDetails final : public FToroStructCustomization
{
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow, 
		IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		const FString FilterTag = StructHandle->GetMetaData(FToroDatabaseDetails::NAME_TagFilter);

		GET_STRUCT_PROPERTY_VAR_NS(Tag, Tag);
		HeaderRow.NameContent() 
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SGameplayTagCombo)
				.Filter(FilterTag)
				.PropertyHandle(Tag)
		];
	}
};
#undef STRUCT_NAME