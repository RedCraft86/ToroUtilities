// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Widgets/ToroSettingsWidget.h"
#include "UserSettings/Providers/UserSettingsProvider.h"
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_SettingRowHover, "Event.SettingHover")

UToroSettingsWidget::UToroSettingsWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UToroSettingsWidget::OnExitClicked()
{
	UToroGameUserSettings::Get()->ApplySettings(false);
	DeactivateWidget();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
void UToroSettingsWidget::OnAutoAdjustClicked()
{
	UToroGameUserSettings::Get()->AutoAdjustScalability();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UToroSettingsWidget::OnRowHovered(const FAsyncMessage& Message)
{
	if (const FSettingRowDescriptor* Descriptor = Message.GetPayloadData<const FSettingRowDescriptor>())
	{
		SettingLabel->SetText(Descriptor->DisplayName);
		SettingDesc->SetText(Descriptor->Description);
		SettingCost->SetText(Descriptor->Performance);
	}
}

void UToroSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ExitButton->OnClicked().AddUObject(this, &UToroSettingsWidget::OnExitClicked);
	AutoAdjustButton->OnClicked().AddUObject(this, &UToroSettingsWidget::OnAutoAdjustClicked);

	const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem
		::GetSharedMessageSystem<FAsyncGameplayMessageSystem>(GetWorld());
	if (System.IsValid())
	{
		HoverListenerHandle = System->BindListener<UToroSettingsWidget>(
			FAsyncMessageId(TAG_SettingRowHover.GetTag()), 
			this, &UToroSettingsWidget::OnRowHovered);
	}
}

void UToroSettingsWidget::NativeDestruct()
{
	Super::NativeDestruct();
	const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem
		::GetSharedMessageSystem<FAsyncGameplayMessageSystem>(GetWorld());
	if (System.IsValid())
	{
		System->UnbindListener(HoverListenerHandle);
	}
	Super::NativeDestruct();
}
