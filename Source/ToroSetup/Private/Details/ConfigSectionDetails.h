// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#pragma once

#include "DetailsPanel/ToroStructCustomization.h"
#include "Widgets/Input/STextComboBox.h"
#include "Types/ConfigInfo.h"
#include "ConfigKeyCache.h"

#define STRUCT_NAME FTSetupConfigSection
class FTSetupConfigSectionDetails final : public FToroStructCustomization
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
		FTSetupConfigKeyCache::ForEachSection(OptionSource, [this](const FString& Section)
		{
			Options.Add(MakeShared<FString>(Section));
		});

		FString Section;
		GET_STRUCT_PROPERTY_VAR_NS(Section, SecHandle);
		SecHandle->GetValue(Section);

		TSharedPtr<FString> Selected = Options[0];
		for (const TSharedPtr<FString>& Option : Options)
		{
			if (*Option == Section)
			{
				Selected = Option;
				break;
			}
		}

		StructBuilder.AddCustomRow(INVTEXT("Section"))
		.NameContent()
		[
			SecHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(STextComboBox)
			.OptionsSource(&Options)
			.InitiallySelectedItem(Selected)
			// ReSharper disable once CppPassValueParameterByConstReference
			.OnSelectionChanged_Lambda([SecHandle](TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo)
			{
				if (SecHandle.IsValid())
				{
					SecHandle->SetValue(*NewValue);
				}
			})
		];

		GET_STRUCT_PROPERTY_VAR_NS(Entries, EntriesHandle);
		StructBuilder.AddProperty(EntriesHandle.ToSharedRef());
	}
};
#undef STRUCT_NAME