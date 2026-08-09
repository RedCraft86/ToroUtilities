// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "Widgets/Input/STextComboBox.h"
#include "Types/ConfigInfo.h"
#include "ConfigKeyCache.h"

#define STRUCT_NAME FTSetupConfigEntry
class FTSetupConfigEntryDetails final : public FToroStructCustomization
{
	TArray<TSharedPtr<FString>> Options;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle, IDetailChildrenBuilder& StructBuilder,
		IPropertyTypeCustomizationUtils& StructCustomizationUtils) override
	{
		FToroStructCustomization::CustomizeChildren(StructHandle, StructBuilder, StructCustomizationUtils);

		FString OptionSource;
		GET_STRUCT_PROPERTY_VAR_NS(OptionSource, SrcHandle);
		SrcHandle->GetValue(OptionSource);

		Options.Empty();
		Options.Add(MakeShared<FString>());
		FTSetupConfigKeyCache::ForEachEntry(OptionSource, [this](const FString& Key)
		{
			Options.Add(MakeShared<FString>(Key));
		});

		FString Key;
		GET_STRUCT_PROPERTY_VAR_NS(Key, KeyHandle);
		KeyHandle->GetValue(Key);

		TSharedPtr<FString> Selected = Options[0];
		for (const TSharedPtr<FString>& Option : Options)
		{
			if (*Option == Key)
			{
				Selected = Option;
				break;
			}
		}

		StructBuilder.AddCustomRow(INVTEXT("Key"))
		.NameContent()
		[
			KeyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(STextComboBox)
			.OptionsSource(&Options)
			.InitiallySelectedItem(Selected)
			// ReSharper disable once CppPassValueParameterByConstReference
			.OnSelectionChanged_Lambda([KeyHandle](TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
			{
				if (KeyHandle.IsValid())
				{
					KeyHandle->SetValue(*NewValue);
				}
			})
		];
	}
};
#undef STRUCT_NAME