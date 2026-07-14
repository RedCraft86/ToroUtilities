// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Widgets/ToroSettingRow.h"
#include "UserSettings/Widgets/ToroSettingsWidget.h"
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
#include "Animation/WidgetAnimation.h"
#include "Components/SpinBox.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"
#include "ToroRuntime.h"
#include "Components/ComboBoxString.h"
#if WITH_EDITOR
#include "Editor/WidgetCompilerLog.h"
#endif

UToroSettingRowBase::UToroSettingRowBase(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UToroSettingRowBase::OnResetClicked()
{
	if (Provider.IsValid())
	{
		Provider.GetMutable().ResetSetting();
		OnSettingsApplied(nullptr, EUserSettingApplyType::UIRefresh);
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRowBase::OnSettingsApplied(const UToroGameUserSettings* Settings, const EUserSettingApplyType Type)
{
	static TFrameValue<bool> bHasRanThisFrame(false);
	if (!bHasRanThisFrame.IsSet() && (Provider.Get().bUpdateDynamically || Type == EUserSettingApplyType::UIRefresh))
	{
		bHasRanThisFrame = true;
		UpdateSettingRow();
	}
}

void UToroSettingRowBase::CheckResettability() const
{
	if (const FUserSettingsProviderBase* ProviderPtr = Provider.GetPtr())
	{
		RevertButton->SetIsEnabled(ProviderPtr->IsResettable());
		RevertButton->SetVisibility(RevertButton->GetIsEnabled() 
			? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRowBase::NativeConstruct()
{
	Super::NativeConstruct();
	RevertButton->OnClicked().AddUObject(this, &UToroSettingRowBase::OnResetClicked);
	UToroGameUserSettings::Get()->OnSettingsApplied.AddUObject(this, &UToroSettingRowBase::OnSettingsApplied);
	UpdateSettingRow();
}

void UToroSettingRowBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (SettingLabel && Provider.IsValid())
	{
		SettingLabel->SetText(Provider.Get().DisplayName);
	}
}

void UToroSettingRowBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (const FUserSettingsProviderBase* ProviderPtr = Provider.GetPtr())
	{
		const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem
		   ::GetSharedMessageSystem<FAsyncGameplayMessageSystem>(GetWorld());
		if (System.IsValid())
		{
			System->QueueMessageForBroadcast(
				FAsyncMessageId(TAG_SettingRowHover.GetTag()), 
				FInstancedStruct::Make<FSettingRowDescriptor>(
					ProviderPtr->DisplayName, 
					ProviderPtr->Description, 
					ProviderPtr->GetPerformanceLabel()
				)
			);
		}
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

#if WITH_EDITOR
void UToroSettingRowBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);
	if (!AllowedStruct || !Provider.GetScriptStruct()->IsChildOf(AllowedStruct))
	{
		CompileLog.Error(FText::Format(INVTEXT("{0}'s provider isn't of allowed type: {1}!"),
			FText::FromString(GetName()), FText::FromString(GetNameSafe(AllowedStruct))));
	}
}
#endif

UToroSettingRow_Toggle::UToroSettingRow_Toggle(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_Bool::StaticStruct();
}

void UToroSettingRow_Toggle::OnToggleClicked()
{
	if (FUserSettingsProvider_Bool* ProviderPtr = Provider.GetMutablePtr<FUserSettingsProvider_Bool>())
	{
		const bool bCurrent = ProviderPtr->GetValue();
		ProviderPtr->SetValue(!bCurrent);

		const float StartTime = bCurrent ? ToggleAnim->GetStartTime() : ToggleAnim->GetEndTime();
		PlayAnimation(ToggleAnim, StartTime, 1, bCurrent ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
		CheckResettability();
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Toggle::UpdateSettingRow()
{
	if (const FUserSettingsProvider_Bool* ProviderPtr = Provider.GetPtr<FUserSettingsProvider_Bool>())
	{
		StopAnimation(ToggleAnim);
		if (const TSharedPtr<FWidgetAnimationState> AnimState = GetOrAddAnimationState(ToggleAnim))
		{
			AnimState->SetCurrentTime(ProviderPtr->GetValue() ? ToggleAnim->GetStartTime() : ToggleAnim->GetEndTime());
		}

		CheckResettability();
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Toggle::NativeConstruct()
{
	Super::NativeConstruct();
	ToggleButton->OnClicked().AddUObject(this, &UToroSettingRow_Toggle::OnToggleClicked);
}

UToroSettingRow_Slider::UToroSettingRow_Slider(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_Float::StaticStruct();
}

void UToroSettingRow_Slider::OnValueChanged(float InValue)
{
	static TFrameValue<bool> bHasRanThisFrame(false);
	if (bHasRanThisFrame.IsSet())
	{
		return;
	}

	if (FUserSettingsProvider_Float* ProviderPtr = Provider.GetMutablePtr<FUserSettingsProvider_Float>())
	{
		ProviderPtr->SetValue(InValue);
		CheckResettability();
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Slider::OnValueCommitted(float InValue, ETextCommit::Type CommitMethod)
{
	OnValueChanged(InValue);
}

void UToroSettingRow_Slider::UpdateSettingRow()
{
	if (const FUserSettingsProvider_Float* ProviderPtr = Provider.GetPtr<FUserSettingsProvider_Float>())
	{
		SpinSlider->SetValue(ProviderPtr->GetValue());
		SpinSlider->SetMinValue(ProviderPtr->MinValue);
		SpinSlider->SetMaxValue(ProviderPtr->MaxValue);
		SpinSlider->SetMinFractionalDigits(ProviderPtr->NumDecimals);
		SpinSlider->SetMaxFractionalDigits(ProviderPtr->NumDecimals);
		SpinSlider->SetDelta(ProviderPtr->GetValueDelta());
		CheckResettability();
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Slider::NativeConstruct()
{
	Super::NativeConstruct();
	SpinSlider->OnValueChanged.AddDynamic(this, &UToroSettingRow_Slider::OnValueChanged);
	SpinSlider->OnValueCommitted.AddDynamic(this, &UToroSettingRow_Slider::OnValueCommitted);
}

UToroSettingRow_Swapper::UToroSettingRow_Swapper(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_IntSwap::StaticStruct();
}

void UToroSettingRow_Swapper::OnLeftButtonClicked()
{
	if (FUserSettingsProvider_IntSwap* ProviderPtr = Provider.GetMutablePtr<FUserSettingsProvider_IntSwap>())
	{
		const uint8 Current = ProviderPtr->GetValue();
		if (Current > 0)
		{
			ProviderPtr->SetValue(Current - 1);

			RightButton->SetIsEnabled(true);
			if (Current == 0)
			{
				LeftButton->SetIsEnabled(false);
			}

			UpdateSettingRow();
		}
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Swapper::OnRightButtonClicked()
{
	if (FUserSettingsProvider_IntSwap* ProviderPtr = Provider.GetMutablePtr<FUserSettingsProvider_IntSwap>())
	{
		const uint8 Current = ProviderPtr->GetValue();
		const uint8 MaxIndex = ProviderPtr->OptionNames.Num() - 1;
		if (Current < MaxIndex)
		{
			ProviderPtr->SetValue(Current + 1);

			LeftButton->SetIsEnabled(true);
			if (Current == MaxIndex)
			{
				RightButton->SetIsEnabled(false);
			}

			UpdateSettingRow();
		}
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Swapper::UpdateSettingRow()
{
	if (const FUserSettingsProvider_IntSwap* ProviderPtr = Provider.GetPtr<FUserSettingsProvider_IntSwap>())
	{
		OptionLabel->SetText(FText::FromString(ProviderPtr->OptionNames[ProviderPtr->GetValue()]));
		CheckResettability();
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Swapper::NativeConstruct()
{
	Super::NativeConstruct();
	LeftButton->OnClicked().AddUObject(this, &UToroSettingRow_Swapper::OnLeftButtonClicked);
	RightButton->OnClicked().AddUObject(this, &UToroSettingRow_Swapper::OnRightButtonClicked);
}

UToroSettingRow_Selector::UToroSettingRow_Selector(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
	AllowedStruct = FUserSettingsProvider_String::StaticStruct();
}

// ReSharper disable once CppPassValueParameterByConstReference
void UToroSettingRow_Selector::OnValueSelected(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::Direct)
	{
		return;
	}

	if (FUserSettingsProvider_String* ProviderPtr = Provider.GetMutablePtr<FUserSettingsProvider_String>())
	{
		ProviderPtr->SetValue(SelectedItem);
		CheckResettability();
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Selector::UpdateSettingRow()
{
	if (FUserSettingsProvider_String* ProviderPtr = Provider.GetMutablePtr<FUserSettingsProvider_String>())
	{
		SelectorBox->ClearOptions();
		for (const FString& Option : ProviderPtr->GetOptions())
		{
			SelectorBox->AddOption(Option);
		}

		SelectorBox->SetSelectedOption(ProviderPtr->GetValue());
		CheckResettability();
	}
	else
	{
		UE_LOG(LogToroRuntime, Error, TEXT("Setting Row %s has no valid provider."), *GetName())
	}
}

void UToroSettingRow_Selector::NativeConstruct()
{
	Super::NativeConstruct();
	SelectorBox->OnSelectionChanged.AddDynamic(this, &UToroSettingRow_Selector::OnValueSelected);
}
