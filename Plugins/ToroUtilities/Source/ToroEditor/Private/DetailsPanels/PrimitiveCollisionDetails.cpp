// Copyright (C) RedCraft86. All Rights Reserved.

#include "PrimitiveCollisionDetails.h"
#include "Helpers/CollisionHelpers.h"
#include "IDetailChildrenBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDocumentation.h"

#define LOCTEXT_NAMESPACE "BodyInstanceCustomization"

void FPrimitiveCollisionDetails::CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TArray<const void*> StructPointers;
	StructPropertyHandle->AccessRawData(StructPointers);
	if (StructPointers.IsEmpty()) return;
	bEnabled = StructPointers.Num() == 1;

	if (!bEnabled)
	{
		FSlateFontInfo MultipleSelectedFont = IDetailLayoutBuilder::GetDetailFont();
		MultipleSelectedFont.Size = 10;
		
		HeaderRow.NameContent()[StructPropertyHandle->CreatePropertyNameWidget()]
		.ValueContent()
		[
			SNew(STextBlock)
			.Font(MultipleSelectedFont)
			.Text(LOCTEXT("MultipleSelectedLabel", "Multiple Selected - unable to modify"))
		];

		return;
	}
	
	StructPtr = (FPrimitiveCollision*)StructPointers[0];
	if (!StructPtr) return;
	
	if (StructPtr->GetResponsesMap().IsEmpty())
		StructPtr->FillResponsesMap();

	ReloadPresets();
	int32 ActiveIdx = 0;
	if (bEnabled)
	{
		for (int i = 0; i < Presets.Num(); i++)
		{
			if (*Presets[i] == StructPtr->GetProfileName())
			{
				ActiveIdx = i;
				break;
			}
		}
	}

	HeaderRow.NameContent()[StructPropertyHandle->CreatePropertyNameWidget()]
	.OverrideResetToDefault(FResetToDefaultOverride::Create(
		TAttribute<bool>::CreateLambda([this]() -> bool {
			return StructPtr && bEnabled ? StructPtr->GetProfileName() != UCollisionProfile::BlockAll_ProfileName : false;
		}),
		FSimpleDelegate::CreateLambda([this, StructPropertyHandle]() {
			OnPresetSelectionChanged(MakeShared<FName>("BlockAll"), ESelectInfo::OnMouseClick, StructPropertyHandle);
		})
	))
	.ValueContent()
	[
		SNew(SComboBox<TSharedPtr<FName>>)
		.OptionsSource(&Presets)
		.InitiallySelectedItem(Presets[ActiveIdx])
		.OnGenerateWidget(this, &FPrimitiveCollisionDetails::OnGeneratePresetWidget)
		.OnSelectionChanged(this, &FPrimitiveCollisionDetails::OnPresetSelectionChanged, StructPropertyHandle)
		.IsEnabled(TAttribute<bool>::CreateLambda([this]() -> bool { return bEnabled; }))
		.Content()
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Text(this, &FPrimitiveCollisionDetails::GetProfileLabel, StructPropertyHandle)
			.ToolTipText(this, &FPrimitiveCollisionDetails::GetProfileTooltip, StructPropertyHandle)
		]
	];
}

void FPrimitiveCollisionDetails::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle,
	IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	if (!bEnabled || !StructPtr) return;
	
	ReloadChannels();

	constexpr float RowWidth = 50.0f;
	const FSlateRenderTransform RowOffset = FSlateRenderTransform(FVector2D(-15.0f, 0.0f));
	const TAttribute<bool> bCanCustomize(this, &FPrimitiveCollisionDetails::CanCustomize);

	const TSharedPtr<IPropertyHandle> CollisionEnabledHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPrimitiveCollision, CollisionEnabled));
	if (CollisionEnabledHandle.IsValid()) StructBuilder.AddProperty(CollisionEnabledHandle.ToSharedRef()).IsEnabled(bCanCustomize);
	
	const TSharedPtr<IPropertyHandle> ObjectTypeHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FPrimitiveCollision, ObjectType));
	if (ObjectTypeHandle.IsValid()) StructBuilder.AddProperty(ObjectTypeHandle.ToSharedRef()).IsEnabled(bCanCustomize);

	StructBuilder.AddCustomRow(FText::GetEmpty()).IsEnabled(bCanCustomize)
	.ValueContent()
	[
		SNew(SHorizontalBox)
		.RenderTransform(RowOffset)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(RowWidth)
			.HAlign(HAlign_Center)
			.Content()
			[
				SNew(STextBlock)
				.Font(IDetailLayoutBuilder::GetDetailFontBold())
				.Text(LOCTEXT("IgnoreCollisionLabel", "Ignore"))
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(RowWidth)
			.HAlign(HAlign_Center)
			.Content()
			[
				SNew(STextBlock)
				.Font(IDetailLayoutBuilder::GetDetailFontBold())
				.Text(LOCTEXT("OverlapCollisionLabel", "Overlap"))
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(RowWidth)
			.HAlign(HAlign_Center)
			.Content()
			[
				SNew(STextBlock)
				.Font(IDetailLayoutBuilder::GetDetailFontBold())
				.Text(LOCTEXT("BlockCollisionLabel", "Block"))
			]
		]
	];

	StructBuilder.AddCustomRow(LOCTEXT("CollisionResponsesLabel", "Collision Responses"))
	.NameContent()
	[
		SNew(SHorizontalBox)
		.RenderTransform(RowOffset)
		+SHorizontalBox::Slot()
		.Padding(2.0f)
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(STextBlock)
				.Font(IDetailLayoutBuilder::GetDetailFontBold())
				.Text(LOCTEXT("CollisionResponsesLabel", "Collision Responses"))
				.ToolTipText(LOCTEXT("CollisionResponsesTooltip", "Override Responses for All Channels"))
		]
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			IDocumentation::Get()->CreateAnchor(TEXT("Engine/Physics/Collision"))
		]
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		.RenderTransform(RowOffset)
		+SHorizontalBox::Slot()
		.AutoWidth()
			[
			SNew(SBox)
			.WidthOverride(RowWidth)
			.HAlign(HAlign_Center)
			.Content()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToAllChannels, ECR_Ignore, StructPropertyHandle)
				.IsChecked(this, &FPrimitiveCollisionDetails::DoesAllChannelsHaveResponse, ECR_Ignore)
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(RowWidth)
			.HAlign(HAlign_Center)
			.Content()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToAllChannels, ECR_Overlap, StructPropertyHandle)
				.IsChecked(this, &FPrimitiveCollisionDetails::DoesAllChannelsHaveResponse, ECR_Overlap)
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(RowWidth)
			.HAlign(HAlign_Center)
			.Content()
			[
				SNew(SCheckBox)
				.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToAllChannels, ECR_Block, StructPropertyHandle)
				.IsChecked(this, &FPrimitiveCollisionDetails::DoesAllChannelsHaveResponse, ECR_Block)
			]
		]
	].IsEnabled(bCanCustomize);

	StructBuilder.AddCustomRow(LOCTEXT("TraceResponses", "Trace Responses")).IsEnabled(bCanCustomize)
	.WholeRowContent().HAlign(HAlign_Center).VAlign(VAlign_Center)
	[
		SNew(STextBlock)
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
		.Text(LOCTEXT("TraceResponses", "Trace Responses"))
		.ToolTipText(LOCTEXT("TraceResponsesTooltip", "Collision Responses for Trace Channels"))
	];

	for (FCollisionChannelInfo TraceChannel : ValidTraceChannels)
	{
		StructBuilder.AddCustomRow(FText::FromString(TraceChannel.DisplayName))
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
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(RowWidth)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToChannel, TraceChannel.CollisionChannel, ECR_Ignore, StructPropertyHandle)
					.IsChecked(this, &FPrimitiveCollisionDetails::DoesChannelHaveResponse, TraceChannel.CollisionChannel, ECR_Ignore)
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(RowWidth)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToChannel, TraceChannel.CollisionChannel, ECR_Overlap, StructPropertyHandle)
					.IsChecked(this, &FPrimitiveCollisionDetails::DoesChannelHaveResponse, TraceChannel.CollisionChannel, ECR_Overlap)
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(RowWidth)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToChannel, TraceChannel.CollisionChannel, ECR_Block, StructPropertyHandle)
					.IsChecked(this, &FPrimitiveCollisionDetails::DoesChannelHaveResponse, TraceChannel.CollisionChannel, ECR_Block)
				]
			]
		].IsEnabled(bCanCustomize);
	}

	StructBuilder.AddCustomRow(LOCTEXT("ObjectResponses", "Object Responses")).IsEnabled(bCanCustomize)
	.WholeRowContent().HAlign(HAlign_Center).VAlign(VAlign_Center)
	[
		SNew(STextBlock)
		.Font(IDetailLayoutBuilder::GetDetailFontBold())
		.Text(LOCTEXT("ObjectResponses", "Object Responses"))
		.ToolTipText(LOCTEXT("ObjectResponsesTooltip", "Collision Responses for Object Channels"))
	];

	for (FCollisionChannelInfo ObjectChannel : ValidObjectChannels)
	{
		StructBuilder.AddCustomRow(FText::FromString(ObjectChannel.DisplayName))
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
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(RowWidth)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToChannel, ObjectChannel.CollisionChannel, ECR_Ignore, StructPropertyHandle)
					.IsChecked(this, &FPrimitiveCollisionDetails::DoesChannelHaveResponse, ObjectChannel.CollisionChannel, ECR_Ignore)
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(RowWidth)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToChannel, ObjectChannel.CollisionChannel, ECR_Overlap, StructPropertyHandle)
					.IsChecked(this, &FPrimitiveCollisionDetails::DoesChannelHaveResponse, ObjectChannel.CollisionChannel, ECR_Overlap)
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(RowWidth)
				.HAlign(HAlign_Center)
				.Content()
				[
					SNew(SCheckBox)
					.OnCheckStateChanged(this, &FPrimitiveCollisionDetails::SetResponseToChannel, ObjectChannel.CollisionChannel, ECR_Block, StructPropertyHandle)
					.IsChecked(this, &FPrimitiveCollisionDetails::DoesChannelHaveResponse, ObjectChannel.CollisionChannel, ECR_Block)
				]
			]
		].IsEnabled(bCanCustomize);
	}
}

void FPrimitiveCollisionDetails::ReloadPresets()
{
	if (const UCollisionProfile* Profile = UCollisionProfile::Get())
	{
		Presets.Empty(Profile->GetNumOfProfiles() + 1);
		Presets.Add(MakeShared<FName>("Custom..."));

		TArray<TSharedPtr<FName>> ValidPresets;
		Profile->GetProfileNames(ValidPresets);

		Presets.Append(ValidPresets);
	}
}

TSharedRef<SWidget> FPrimitiveCollisionDetails::OnGeneratePresetWidget(TSharedPtr<FName> InOption) const
{
	return SNew(STextBlock).Font(IDetailLayoutBuilder::GetDetailFont()).Text(FText::FromName(*InOption));
}

void FPrimitiveCollisionDetails::OnPresetSelectionChanged(TSharedPtr<FName> NewSelection, ESelectInfo::Type SelectInfo, const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (!StructPtr || !bEnabled) return;
	StructHandle->NotifyPreChange();
	StructPtr->SetProfileName(*NewSelection);
	StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
}

FText FPrimitiveCollisionDetails::GetProfileLabel(const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (!StructPtr) return LOCTEXT("UnknownValue", "Unknown Value");

	const FProperty* PropertyValue;
	if (StructHandle->GetValue(PropertyValue) == FPropertyAccess::MultipleValues)
	{
		return LOCTEXT("MultipleValues", "Multiple Values");
	}

	FCollisionResponseTemplate Template;
	if (UCollisionProfile::Get()->GetProfileTemplate(StructPtr->GetProfileName(), Template))
	{
		return FText::FromName(Template.Name);
	}
	
	return LOCTEXT("Custom", "Custom...");
}

FText FPrimitiveCollisionDetails::GetProfileTooltip(const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (!StructPtr) return LOCTEXT("UnknownValue", "Unknown Value");

	const FProperty* PropertyValue;
	if (StructHandle->GetValue(PropertyValue) == FPropertyAccess::MultipleValues)
	{
		return LOCTEXT("MultipleValues", "Multiple Values");
	}

	FCollisionResponseTemplate Template;
	if (UCollisionProfile::Get()->GetProfileTemplate(StructPtr->GetProfileName(), Template))
	{
		return FText::FromString(Template.HelpMessage);
	}

	return LOCTEXT("CustomProfile", "Custom Collision Profile");
}

void FPrimitiveCollisionDetails::ReloadChannels()
{
	ValidTraceChannels.Empty();
	ValidObjectChannels.Empty();
	CollisionHelpers::ForEachChannel([this](const ECollisionChannel Value, const int32 Index, const UEnum* EnumPtr)
	{
		const FCollisionChannelInfo Info{
			EnumPtr->GetNameStringByIndex(Index),
			EnumPtr->GetMetaData(TEXT("DisplayName"), Index),
			Value
		};

		if (EnumPtr->GetMetaData(TEXT("TraceQuery"), Index) == TEXT("1"))
		{
			ValidTraceChannels.Add(Info);
		}
		else
		{
			ValidObjectChannels.Add(Info);
		}
	}, true);
}

bool FPrimitiveCollisionDetails::CanCustomize() const
{
	if (!StructPtr || !bEnabled) return false;
	return StructPtr->GetProfileName() == UCollisionProfile::CustomCollisionProfileName;
}

void FPrimitiveCollisionDetails::SetResponseToAllChannels(ECheckBoxState InCheckState,
	const ECollisionResponse NewResponse, const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (!StructPtr || !bEnabled) return;
	StructHandle->NotifyPreChange();
	StructPtr->SetAllResponses(NewResponse);
	StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
}

ECheckBoxState FPrimitiveCollisionDetails::DoesAllChannelsHaveResponse(const ECollisionResponse ResponseToCheck) const
{
	if (!StructPtr || !bEnabled) return ECheckBoxState::Undetermined;
	
	bool bHasAny = false;
	bool bHasMixed = false;
	CollisionHelpers::ForEachChannel([&bHasAny, &bHasMixed, this, ResponseToCheck](const ECollisionChannel Channel)
	{
		if (StructPtr->GetResponse(Channel) == ResponseToCheck) bHasAny = true; else bHasMixed = true;
	}, true);

	return bHasAny ? (bHasMixed ? ECheckBoxState::Undetermined : ECheckBoxState::Checked) : ECheckBoxState::Unchecked;
}

void FPrimitiveCollisionDetails::SetResponseToChannel(ECheckBoxState InCheckState, const ECollisionChannel Channel,
	const ECollisionResponse NewResponse, const TSharedRef<IPropertyHandle> StructHandle) const
{
	if (!StructPtr || !bEnabled) return;
	StructHandle->NotifyPreChange();
	StructPtr->SetResponse(Channel, NewResponse);
	StructHandle->NotifyPostChange(EPropertyChangeType::ValueSet);
}

ECheckBoxState FPrimitiveCollisionDetails::DoesChannelHaveResponse(const ECollisionChannel Channel, const ECollisionResponse ResponseToCheck) const
{
	if (!StructPtr || !bEnabled) return ECheckBoxState::Undetermined;
	return StructPtr->GetResponse(Channel) == ResponseToCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

#undef LOCTEXT_NAMESPACE