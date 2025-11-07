// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/Widgets/SettingRowWidgets.h"
#include "Helpers/WidgetAnimHelpers.h"

UToggleSettingRow::UToggleSettingRow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bState(false)
{
}

void UToggleSettingRow::ToggleClicked()
{
	if (FToggleOptionBinding* Ptr = Binding.GetMutablePtr())
	{
		bState = !bState;
		Ptr->SetValue(bState);
		SyncVisualState(false);
	}
}

void UToggleSettingRow::SyncVisualState(const bool bImmediate)
{
	WidgetAnimHelpers::PlayOrSnapAnim(this, ToggleAnim, bState, bImmediate);
}

void UToggleSettingRow::OnRefreshUI()
{
	if (FToggleOptionBinding* Ptr = Binding.GetMutablePtr())
	{
		Ptr->InitBinding();
		bState = Ptr->GetValue();
		
		SyncVisualState(true);
		SetToolTipText(Ptr->GetFormattedTooltip());
		SetDisabled(Ptr->ShouldHide());
	}
}

FText UToggleSettingRow::GetLabelText()
{
	const FToggleOptionBinding* Ptr = Binding.GetPtr();
	return Ptr ? Ptr->Name : Super::GetLabelText();
}

void UToggleSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	ToggleButton->OnClicked.AddUniqueDynamic(this, &UToggleSettingRow::ToggleClicked);
}

USliderSettingRow::USliderSettingRow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bOverriding(false)
{
}

void USliderSettingRow::SetValue(const float InValue)
{
	FSliderOptionBinding* Ptr = Binding.GetMutablePtr();
	if (!bOverriding && Ptr)
	{
		Ptr->SetValue(InValue);
	}
}

void USliderSettingRow::OnRefreshUI()
{
	if (FSliderOptionBinding* Ptr = Binding.GetMutablePtr())
	{
		Ptr->InitBinding();
		bOverriding = true;
		SliderBox->SetValue(Ptr->GetValue());
		SliderBox->SetMinValue(Ptr->SliderRange.X);
		SliderBox->SetMaxValue(Ptr->SliderRange.Y);
		SliderBox->SetMinSliderValue(Ptr->SliderRange.X);
		SliderBox->SetMaxSliderValue(Ptr->SliderRange.Y);
		SliderBox->SetMaxFractionalDigits(Ptr->MaxDecimals);
		SliderBox->SetDelta(FMath::Pow(10.0f, -Ptr->MaxDecimals));
		bOverriding = false;

		SetToolTipText(Ptr->GetFormattedTooltip());
		SetDisabled(Ptr->ShouldHide());
	}
}

FText USliderSettingRow::GetLabelText()
{
	const FSliderOptionBinding* Ptr = Binding.GetPtr();
	return Ptr ? Ptr->Name : Super::GetLabelText();
}

void USliderSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	SliderBox->OnEndSliderMovement.AddUniqueDynamic(this, &USliderSettingRow::OnSliderEnd);
	SliderBox->OnValueCommitted.AddUniqueDynamic(this, &USliderSettingRow::OnSliderCommit);
	const FSliderOptionBinding* Ptr = Binding.GetPtr();
	if (Ptr && Ptr->bConstantUpdate)
	{
		SliderBox->OnValueChanged.AddUniqueDynamic(this, &USliderSettingRow::OnSliderDynamic);
	}
}

USwapperSettingRow::USwapperSettingRow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), OptionIdx(0)
{
}

void USwapperSettingRow::RightArrow()
{
	FSwapperOptionBinding* Ptr = Binding.GetMutablePtr();
	if (Ptr && OptionIdx != Options.Num() - 1)
	{
		OptionIdx++;
		Ptr->SetValue(OptionIdx);
		SyncVisualOption();
	}
}

void USwapperSettingRow::LeftArrow()
{
	FSwapperOptionBinding* Ptr = Binding.GetMutablePtr();
	if (Ptr && OptionIdx != 0)
	{
		OptionIdx--;
		Ptr->SetValue(OptionIdx);
		SyncVisualOption();
	}
}

void USwapperSettingRow::SyncVisualOption()
{
	if (Options.IsValidIndex(OptionIdx))
	{
		OptionLabel->SetText(FText::FromName(Options[OptionIdx]));
	}
	else
	{
		OptionLabel->SetText(INVTEXT("Unknown Option"));
	}

	if (const FSwapperOptionBinding* Ptr = Binding.GetPtr())
	{
		SetToolTipText(Ptr->GetFormattedTooltip());
	}
}

void USwapperSettingRow::OnRefreshUI()
{
	if (FSwapperOptionBinding* Ptr = Binding.GetMutablePtr())
	{
		Ptr->InitBinding();
		Options = Ptr->GetOptions();
		OptionIdx = Ptr->GetValue();
		
		SyncVisualOption();
		SetDisabled(Ptr->ShouldHide());
	}
}

FText USwapperSettingRow::GetLabelText()
{
	const FSwapperOptionBinding* Ptr = Binding.GetPtr();
	return Ptr ? Ptr->Name : Super::GetLabelText();
}

void USwapperSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	RightButton->OnClicked.AddUniqueDynamic(this, &USwapperSettingRow::RightArrow);
	LeftButton->OnClicked.AddUniqueDynamic(this, &USwapperSettingRow::LeftArrow);
}

UDropdownSettingRow::UDropdownSettingRow(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bOverriding(false)
{
}

void UDropdownSettingRow::OptionPicked(const FString Option, ESelectInfo::Type Info)
{
	FDropdownOptionBinding* Ptr = Binding.GetMutablePtr();
	if (!bOverriding && Ptr)
	{
		Ptr->SetValue(Option);
		SetToolTipText(Ptr->GetFormattedTooltip());
	}
}

void UDropdownSettingRow::OnRefreshUI()
{
	if (FDropdownOptionBinding* Ptr = Binding.GetMutablePtr())
	{
		Ptr->InitBinding();
		bOverriding = true;
		DropdownBox->ClearOptions();
		TArray<FString> Options = Ptr->GetOptions();
		for (const FString& Option : Options)
		{
			DropdownBox->AddOption(Option);
		}
		DropdownBox->SetSelectedOption(Ptr->GetValue());
		bOverriding = false;

		SetToolTipText(Ptr->GetFormattedTooltip());
		SetDisabled(Ptr->ShouldHide());
	}
}

FText UDropdownSettingRow::GetLabelText()
{
	const FDropdownOptionBinding* Ptr = Binding.GetPtr();
	return Ptr ? Ptr->Name : Super::GetLabelText();
}

void UDropdownSettingRow::NativeConstruct()
{
	Super::NativeConstruct();
	DropdownBox->OnSelectionChanged.AddUniqueDynamic(this, &UDropdownSettingRow::OptionPicked);
}
