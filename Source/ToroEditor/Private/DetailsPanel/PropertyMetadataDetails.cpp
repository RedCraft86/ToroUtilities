// Copyright (C) 2026 Tayzar Linn. Licensed under GNU Lesser General Public License v3.0, see project LICENSE file.

#include "PropertyMetadataDetails.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "PropertyCustomizationHelpers.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailGroup.h"

#define LOCTEXT_NAMESPACE "ToroEditor"

TSharedPtr<IDetailCustomization> FPropertyMetadataDetails::MakeInstance(TSharedPtr<IBlueprintEditor> InBlueprintEd)
{
	if (!InBlueprintEd.IsValid())
	{
		return nullptr;
	}

	const TArray<UObject*>* Objects = InBlueprintEd->GetObjectsCurrentlyBeingEdited();
	if (UBlueprint* Blueprint = (Objects && Objects->Num() == 1) ? Cast<UBlueprint>((*Objects)[0]) : nullptr)
	{
		return MakeShared<FPropertyMetadataDetails>(InBlueprintEd, Blueprint);
	}

	return nullptr;
}

int32 FPropertyMetadataDetails::GetNumMetadata() const
{
	return PropIdx != INDEX_NONE ? BlueprintPtr->NewVariables[PropIdx].MetaDataArray.Num() : 0;
}

bool FPropertyMetadataDetails::HasMetadata(const FName Key) const
{
	return PropIdx != INDEX_NONE && !Key.IsNone() ? BlueprintPtr->NewVariables[PropIdx].HasMetaData(Key) : false;
}

void FPropertyMetadataDetails::VerifyMetadata(const FName Key) const
{
	if (HasMetadata(Key) && GetMetadata(Key).IsEmpty())
	{
		RemoveMedata(Key);
	}
}

void FPropertyMetadataDetails::RemoveMedata(const FName Key) const
{
	if (HasMetadata(Key))
	{
		FBlueprintEditorUtils::RemoveBlueprintVariableMetaData(BlueprintPtr.Get(), PropName, nullptr, Key);
	}
}

FString FPropertyMetadataDetails::GetMetadata(const FName Key) const
{
	if (HasMetadata(Key))
	{
		FString Value;
		FBlueprintEditorUtils::GetBlueprintVariableMetaData(BlueprintPtr.Get(), PropName, nullptr, Key, Value);
		return Value;
	}

	return FString();
}

void FPropertyMetadataDetails::SetMetadata(const FName Key, const FString& Value) const
{
	if (!Key.IsNone())
	{
		FBlueprintEditorUtils::SetBlueprintVariableMetaData(BlueprintPtr.Get(), PropName, nullptr, Key, Value);
	}
}

void FPropertyMetadataDetails::ChangeMetadataKey(const FName OldKey, const FName NewKey) const
{
	if (HasMetadata(OldKey))
	{
		if (!NewKey.IsNone())
		{
			SetMetadata(NewKey, GetMetadata(OldKey));
		}

		RemoveMedata(OldKey);
	}
}

void FPropertyMetadataDetails::AddNewMetadata() const
{
	SetMetadata(*FString::Printf(TEXT("NewMeta_%d"), GetNumMetadata()), "");
}

void FPropertyMetadataDetails::OnChangeMetaKey(const FText& NewKey, ETextCommit::Type Type, const FName OldKey) const
{
	ChangeMetadataKey(OldKey, *NewKey.ToString());
}

void FPropertyMetadataDetails::OnChangeMetaValue(const FText& NewValue, ETextCommit::Type Type, const FName Key) const
{
	SetMetadata(Key, NewValue.ToString());
}

void FPropertyMetadataDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);
	if (Objects.IsEmpty()) return;

	UPropertyWrapper* PropWrapper = Cast<UPropertyWrapper>(Objects[0]);
	const TWeakFieldPtr<FProperty> Property = PropWrapper ? PropWrapper->GetProperty() : nullptr;
	if (!Property.IsValid()) return;

	PropName = Property->GetFName();
	if (PropName.IsNone()) return;

	PropIdx = FBlueprintEditorUtils::FindNewVariableIndex(BlueprintPtr.Get(), PropName);
	if (PropIdx == INDEX_NONE) return;

	VerifyMetadata(TEXT("Tooltip"));
	VerifyMetadata(TEXT("UIMin"));
	VerifyMetadata(TEXT("UIMax"));
	VerifyMetadata(TEXT("ClampMin"));
	VerifyMetadata(TEXT("ClampMax"));

	IDetailCategoryBuilder& VariableCat = DetailBuilder.EditCategory(TEXT("Variable"));
	IDetailGroup& MetadataGroup = VariableCat.AddGroup(TEXT("MetadataGroup"), LOCTEXT("MetadataGroupName", "Metadata"));
	MetadataGroup.HeaderRow()
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("MetadataListName", "Metadata List"))
		.ToolTipText(LOCTEXT("MetadataListTooltip", "List of Metadata entries for this variable."))
		.Font(IDetailLayoutBuilder::GetDetailFont())
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::Format(INVTEXT("{0} Metadata Entries"), GetNumMetadata()))
			.Font(IDetailLayoutBuilder::GetDetailFont())
		]
		+SHorizontalBox::Slot()
		.Padding(2.0f, 0.0f, 0.0f, 0.0f)
		.AutoWidth()
		[
			PropertyCustomizationHelpers::MakeAddButton(
				FSimpleDelegate::CreateRaw(this, &FPropertyMetadataDetails::AddNewMetadata),
				LOCTEXT("MetadataAddName", "Add Metadata")
			)
		]
	];

	TArray<FBPVariableMetaDataEntry> MetadataEntries = BlueprintPtr->NewVariables[PropIdx].MetaDataArray;
	for (const FBPVariableMetaDataEntry& Entry : MetadataEntries)
	{
		MetadataGroup.AddWidgetRow()
		.NameContent()
		.HAlign(HAlign_Fill)
		[
			SNew(SEditableTextBox)
			.Text(FText::FromName(Entry.DataKey))
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.OnTextCommitted(FOnTextCommitted::CreateRaw(
				this, &FPropertyMetadataDetails::OnChangeMetaKey, Entry.DataKey
			))
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SEditableTextBox)
				.Text(FText::FromString(Entry.DataValue))
				.Font(IDetailLayoutBuilder::GetDetailFont())
				.OnTextCommitted(FOnTextCommitted::CreateRaw(
					this, &FPropertyMetadataDetails::OnChangeMetaValue, Entry.DataKey
				))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(2.0f, 0.0f, 0.0f, 0.0f)
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Fill)
			[
				PropertyCustomizationHelpers::MakeRemoveButton(
					FSimpleDelegate::CreateRaw(this, &FPropertyMetadataDetails::RemoveMedata, Entry.DataKey),
					LOCTEXT("MetadataRemoveName", "Remove Metadata"), true
				)
			]
		];
	}
}
#undef LOCTEXT_NAMESPACE