// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailsPanel/ToroClassCustomization.h"
#include "DataAssets/ToroDatabase.h"

#define CLASS_NAME UToroDatabase
class FToroDatabaseDetails final : public FToroClassCustomization
{
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