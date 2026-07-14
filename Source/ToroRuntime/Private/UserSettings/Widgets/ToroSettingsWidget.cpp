// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Widgets/ToroSettingsWidget.h"
#include "UserSettings/Providers/UserSettingsProvider.h"
#include "AsyncGameplayMessageSystem.h"
#include "AsyncMessageWorldSubsystem.h"
#include "CommonTextBlock.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_SettingRowHover, "Event.SettingHover")

UToroSettingsWidget::UToroSettingsWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UToroSettingsWidget::OnRowHovered(const FAsyncMessage& Message)
{
	if (const FUserSettingsProviderBase* Provider = Message.GetPayloadData<FUserSettingsProviderBase>())
	{
		SettingLabel->SetText(Provider->DisplayName);
		SettingDesc->SetText(Provider->Description);
		SettingCost->SetText(Provider->GetPerformanceLabel());
	}
}

void UToroSettingsWidget::NativeConstruct()
{
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
	const TSharedPtr<FAsyncGameplayMessageSystem> System = UAsyncMessageWorldSubsystem
		::GetSharedMessageSystem<FAsyncGameplayMessageSystem>(GetWorld());
	if (System.IsValid())
	{
		System->UnbindListener(HoverListenerHandle);
	}
	Super::NativeDestruct();
}
