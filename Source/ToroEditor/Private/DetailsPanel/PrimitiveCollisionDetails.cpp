// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "PrimitiveCollisionDetails.h"
#include "DetailLayoutBuilder.h"
#include "IDocumentation.h"
#include "Widgets/Input/SCheckBox.h"

#define LOCTEXT_NAMESPACE "BodyInstanceCustomization"
#define STRUCT_NAME FPrimitiveCollision

FPrimitiveCollision* FPrimitiveCollisionDetails::GetStructPtr(const TSharedRef<IPropertyHandle>& PropertyHandle)
{
	if (StructPtr)
	{
		return StructPtr;
	}

	TArray<void*> StructPointers;
	PropertyHandle->AccessRawData(StructPointers);
	if (StructPointers.Num() == 1)
	{
		StructPtr = static_cast<FPrimitiveCollision*>(StructPointers[0]);
		return StructPtr;
	}

	return nullptr;
}

void FPrimitiveCollisionDetails::OnPresetSelected(TSharedPtr<FName> NewPreset, ESelectInfo::Type SelectInfo, const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (StructPtr && SelectInfo != ESelectInfo::Type::Direct)
	{
		const bool bReset = !NewPreset.IsValid();

		StructHandle->NotifyPreChange();
		StructPtr->SetProfileName(bReset ? *DefaultPreset : *NewPreset);
		StructHandle->NotifyPostChange(bReset ? EPropertyChangeType::ResetToDefault : EPropertyChangeType::ValueSet);
		if (bReset && ComboBoxPtr.IsValid())
		{
			ComboBoxPtr->SetSelectedItem(DefaultPreset);
		}
	}
}

ECheckBoxState FPrimitiveCollisionDetails::HasCommonResponse(const ECollisionResponse ResponseToCheck) const
{
	if (!StructPtr)
	{
		return ECheckBoxState::Undetermined;
	}
	
	ECheckBoxState State = ECheckBoxState::Unchecked;
	FPrimitiveCollision::Utils::ForEachChannel([this, ResponseToCheck, &State]
		(const ECollisionChannel Channel, const int32, const UEnum*)
	{
		if (State == ECheckBoxState::Undetermined) return;
		if (StructPtr->GetResponse(Channel) == ResponseToCheck)
		{
			if (State == ECheckBoxState::Unchecked)
			{
				State = ECheckBoxState::Checked;
			}
		}
		else if (State == ECheckBoxState::Checked)
		{
			State = ECheckBoxState::Undetermined;
		}
	}, true);

	return State;
}

void FPrimitiveCollisionDetails::SetAllResponses(ECheckBoxState CheckState,
	const ECollisionResponse Response, const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (StructPtr)
	{
		StructHandle->NotifyPreChange();
		StructPtr->SetAllResponses(Response);
		StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
	}
}

ECheckBoxState FPrimitiveCollisionDetails::HasResponse(const ECollisionChannel Channel,
	const ECollisionResponse ResponseToCheck) const
{
	if (StructPtr)
	{
		return StructPtr->GetResponse(Channel) == ResponseToCheck
			? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}
	return ECheckBoxState::Undetermined;
}

void FPrimitiveCollisionDetails::SetResponse(ECheckBoxState CheckState, const ECollisionChannel Channel,
	const ECollisionResponse Response, const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (StructPtr)
	{
		StructHandle->NotifyPreChange();
		StructPtr->SetResponse(Channel, Response);
		StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
	}
}

void FPrimitiveCollisionDetails::CustomizeHeader(TSharedRef<IPropertyHandle> StructHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	FToroStructCustomization::CustomizeHeader(StructHandle, HeaderRow, CustomizationUtils);

	if (!GetStructPtr(StructHandle))
	{
		FSlateFontInfo MultipleSelectedFont = IDetailLayoutBuilder::GetDetailFont();
		MultipleSelectedFont.Size = 10;
		
		HeaderRow.NameContent()
		[
			StructHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(STextBlock)
			.Font(MultipleSelectedFont)
			.Text(LOCTEXT("MultipleSelectedLabel", "Multiple Selected"))
		];
		return;
	}

	if (Presets.IsEmpty())
	{
		if (const UCollisionProfile* Profile = UCollisionProfile::Get())
		{
			Profile->GetProfileNames(Presets);
			Presets.Insert(MakeShared<FName>(TEXT("Custom...")), 0);

			if (const TSharedPtr<FName>* Preset = Presets.FindByPredicate([](const TSharedPtr<FName>& Element)
			{
				return *Element == UCollisionProfile::BlockAll_ProfileName;
			}))
			{
				DefaultPreset = *Preset;
			}
			else
			{
				DefaultPreset = Presets[0];
			}
		}
	}

	int32 SelectedPreset;
	if (StructPtr->GetProfileName() == UCollisionProfile::CustomCollisionProfileName)
	{
		SelectedPreset = 0;
		StructPtr->FillAllResponses();
	}
	else
	{
		SelectedPreset = Presets.IndexOfByPredicate([this](const TSharedPtr<FName>& Preset)
		{
			return *Preset == StructPtr->GetProfileName();
		});
	}

	if (SelectedPreset == INDEX_NONE)
	{
		SelectedPreset = 0;
		StructPtr->SetProfileName(NAME_None);
	}

	HeaderRow.OverrideResetToDefault(FResetToDefaultOverride::Create(
		TAttribute<bool>::CreateLambda([this]()
		{
			return StructPtr && StructPtr->GetProfileName() != *DefaultPreset;
		}),
		FSimpleDelegate::CreateLambda([this, StructHandle]()
		{
			OnPresetSelected(nullptr, ESelectInfo::OnMouseClick, StructHandle);
		})
	))
	.NameContent()
	[
		StructHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	[
		SAssignNew(ComboBoxPtr, SNameComboBox)
		.OptionsSource(&Presets)
		.InitiallySelectedItem(Presets[SelectedPreset])
		.OnSelectionChanged(this, &FPrimitiveCollisionDetails::OnPresetSelected, StructHandle)
		.ContentPadding(FMargin(2.0f, 0.0f))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	];
}

void FPrimitiveCollisionDetails::CustomizeChildren(TSharedRef<IPropertyHandle> StructHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	FToroStructCustomization::CustomizeChildren(StructHandle, ChildBuilder, CustomizationUtils);

	if (!GetStructPtr(StructHandle))
	{
		return;
	}

	TraceChannels.Empty();
	ObjectChannels.Empty();
	FPrimitiveCollision::Utils::ForEachChannel([this](const ECollisionChannel Value, const int32 Index, const UEnum* Enum)
	{
		const FCollisionChannelInfo Info(
			Enum->GetNameStringByIndex(Index),
			Enum->GetMetaData(TEXT("DisplayName"), Index),
			Value
		);
		if (Enum->GetMetaData(TEXT("TraceQuery"), Index) == TEXT("1"))
		{
			TraceChannels.Add(Info);
		}
		else
		{
			ObjectChannels.Add(Info);
		}
	}, true);

	const FSlateRenderTransform RowOffset = FSlateRenderTransform(FVector2D(10.0f, 0.0f));
	const TAttribute<bool> bCanCustomize(this, &FPrimitiveCollisionDetails::CanCustomize);

	ChildBuilder.AddProperty(GET_STRUCT_PROPERTY_NS(CollisionEnabled).ToSharedRef()).IsEnabled(bCanCustomize);
	ChildBuilder.AddProperty(GET_STRUCT_PROPERTY_NS(ObjectType).ToSharedRef()).IsEnabled(bCanCustomize);

	ChildBuilder.AddCustomRow(LOCTEXT("PrimitiveCollision", "Primitive Collision")).IsEnabled(bCanCustomize)
	.ValueContent()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFontBold())
			.Text(LOCTEXT("IgnoreCollisionLabel", "Ignore"))
			.Justification(ETextJustify::Center)
		]
		+SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFontBold())
			.Text(LOCTEXT("OverlapCollisionLabel", "Overlap"))
			.Justification(ETextJustify::Center)
		]
		+SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFontBold())
			.Text(LOCTEXT("BlockCollisionLabel", "Block"))
			.Justification(ETextJustify::Center)
		]
	];
	
	ChildBuilder.AddCustomRow(LOCTEXT("CollisionResponsesLabel", "Collision Responses")).IsEnabled(bCanCustomize)
	.NameContent()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.Padding(0.0f, 0.0f, 2.0f, 0.0f)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFontBold())
			.Text(LOCTEXT("CollisionResponsesLabel", "Collision Responses"))
			.ToolTipText(LOCTEXT("CollisionResponsesTooltip", "Override Responses for All Channels"))
		]
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		[
			IDocumentation::Get()->CreateAnchor(TEXT("Engine/Physics/Collision"))
		]
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		.RenderTransform(RowOffset)
		+SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SCheckBox)
			.IsChecked(this, &FPrimitiveCollisionDetails::HasCommonResponse, ECR_Ignore)
			.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetAllResponses, ECR_Ignore, StructHandle)
		]
		+SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SCheckBox)
			.IsChecked(this, &FPrimitiveCollisionDetails::HasCommonResponse, ECR_Overlap)
			.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetAllResponses, ECR_Overlap, StructHandle)
		]
		+SHorizontalBox::Slot()
		.FillWidth(1.0f)
		[
			SNew(SCheckBox)
			.IsChecked(this, &FPrimitiveCollisionDetails::HasCommonResponse, ECR_Block)
			.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetAllResponses, ECR_Block, StructHandle)
		]
	];

	ChildBuilder.AddCustomRow(LOCTEXT("TraceResponses", "Trace Responses")).IsEnabled(bCanCustomize)
	.ValueContent()
	[
		SNew(STextBlock)
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
		.Text(LOCTEXT("TraceResponses", "Trace Responses"))
		.ToolTipText(LOCTEXT("TraceResponsesTooltip", "Collision Responses for Trace Channels"))
		.Justification(ETextJustify::Center)
	];

	for (const FCollisionChannelInfo& TraceChannel : TraceChannels)
	{
		ChildBuilder.AddCustomRow(FText::FromString(TraceChannel.DisplayName)).IsEnabled(bCanCustomize)
		.NameContent()
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Text(FText::FromString(TraceChannel.DisplayName))
			.ToolTipText(FText::FromString(TraceChannel.RawName))
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			.RenderTransform(RowOffset)
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SCheckBox)
				.IsChecked(this, &FPrimitiveCollisionDetails::HasResponse, TraceChannel.Channel, ECR_Ignore)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponse, TraceChannel.Channel, ECR_Ignore, StructHandle)
				.ToolTipText(LOCTEXT("IgnoreCollisionLabel", "Ignore"))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SCheckBox)
				.IsChecked(this, &FPrimitiveCollisionDetails::HasResponse, TraceChannel.Channel, ECR_Overlap)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponse, TraceChannel.Channel, ECR_Overlap, StructHandle)
				.ToolTipText(LOCTEXT("OverlapCollisionLabel", "Overlap"))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SCheckBox)
				.IsChecked(this, &FPrimitiveCollisionDetails::HasResponse, TraceChannel.Channel, ECR_Block)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponse, TraceChannel.Channel, ECR_Block, StructHandle)
				.ToolTipText(LOCTEXT("BlockCollisionLabel", "Block"))
			]
		];
	}

	ChildBuilder.AddCustomRow(LOCTEXT("ObjectResponses", "Object Responses")).IsEnabled(bCanCustomize)
	.ValueContent()
	[
		SNew(STextBlock)
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
		.Text(LOCTEXT("ObjectResponses", "Object Responses"))
		.ToolTipText(LOCTEXT("ObjectResponsesTooltip", "Collision Responses for Object Channels"))
		.Justification(ETextJustify::Center)
	];

	for (const FCollisionChannelInfo& ObjectChannel : ObjectChannels)
	{
		ChildBuilder.AddCustomRow(FText::FromString(ObjectChannel.DisplayName)).IsEnabled(bCanCustomize)
		.NameContent()
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Text(FText::FromString(ObjectChannel.DisplayName))
			.ToolTipText(FText::FromString(ObjectChannel.RawName))
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			.RenderTransform(RowOffset)
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SCheckBox)
				.IsChecked(this, &FPrimitiveCollisionDetails::HasResponse, ObjectChannel.Channel, ECR_Ignore)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponse, ObjectChannel.Channel, ECR_Ignore, StructHandle)
				.ToolTipText(LOCTEXT("IgnoreCollisionLabel", "Ignore"))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SCheckBox)
				.IsChecked(this, &FPrimitiveCollisionDetails::HasResponse, ObjectChannel.Channel, ECR_Overlap)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponse, ObjectChannel.Channel, ECR_Overlap, StructHandle)
				.ToolTipText(LOCTEXT("OverlapCollisionLabel", "Overlap"))
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.0f)
			[
				SNew(SCheckBox)
				.IsChecked(this, &FPrimitiveCollisionDetails::HasResponse, ObjectChannel.Channel, ECR_Block)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponse, ObjectChannel.Channel, ECR_Block, StructHandle)
				.ToolTipText(LOCTEXT("BlockCollisionLabel", "Block"))
			]
		];
	}
}

#undef STRUCT_NAME
#undef LOCTEXT_NAMESPACE