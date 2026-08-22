// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "Widgets/Input/STextComboBox.h"
#include "Types/SelectableString.h"
#include "ConfigKeyCache.h"

#define STRUCT_NAME FTSetupSelectableString
class FTSetupSelectableStringDetails final : public FToroStructCustomization
{
	TArray<TSharedPtr<FString>> Options;

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle, FDetailWidgetRow& HeaderRow,
		IPropertyTypeCustomizationUtils& CustomizationUtils) override
	{
		FToroStructCustomization::CustomizeHeader(StructHandle, HeaderRow, CustomizationUtils);

		FString OptionSource;
		GET_STRUCT_PROPERTY_VAR_NS(OptionSource, SrcHandle);
		SrcHandle->GetValue(OptionSource);

		Options.Empty();
		Options.Add(MakeShared<FString>());
		if (OptionSource.Contains(TEXT(":")))
		{
			FTSetupConfigKeyCache::ForEachEntry(OptionSource, [this](const FString& Section)
			{
				Options.Add(MakeShared<FString>(Section));
			});
		}
		else
		{
			FTSetupConfigKeyCache::ForEachSection(OptionSource, [this](const FString& Section)
			{
				Options.Add(MakeShared<FString>(Section));
			});
		}

		FString Value;
		GET_STRUCT_PROPERTY_VAR_NS(Value, ValHandle);
		ValHandle->GetValue(Value);

		TSharedPtr<FString> Selected = Options[0];
		for (const TSharedPtr<FString>& Option : Options)
		{
			if (*Option == Value)
			{
				Selected = Option;
				break;
			}
		}

		HeaderRow
		.NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(STextComboBox)
			.OptionsSource(&Options)
			.InitiallySelectedItem(Selected)
			// ReSharper disable once CppPassValueParameterByConstReference
			.OnSelectionChanged_Lambda([ValHandle](TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
			{
				if (ValHandle.IsValid())
				{
					ValHandle->SetValue(*NewValue);
				}
			})
		];
	}
};
#undef STRUCT_NAME