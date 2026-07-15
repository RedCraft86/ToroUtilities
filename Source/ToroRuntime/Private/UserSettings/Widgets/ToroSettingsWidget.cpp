// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserSettings/Widgets/ToroSettingsWidget.h"
#include "UserSettings/Providers/UserSettingsProvider.h"
#include "UserSettings/Widgets/ToroSettingRow.h"
#include "Blueprint/WidgetTree.h"
#include "CommonButtonBase.h"
#include "CommonTextBlock.h"

UToroSettingsWidget::UToroSettingsWidget(const FObjectInitializer& ObjectInit): Super(ObjectInit)
{
}

void UToroSettingsWidget::OnExitClicked()
{
	UToroGameUserSettings::Get()->ApplySettings(false);
	DeactivateWidget();
}

void UToroSettingsWidget::OnRowHovered(const FSettingRowDescriptor& Info) const
{
	SettingLabel->SetText(Info.DisplayName);
	SettingDesc->SetText(Info.Description);
	SettingCost->SetText(Info.Performance);
}

void UToroSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ExitButton->OnClicked().AddUObject(this, &UToroSettingsWidget::OnExitClicked);

	WidgetTree->ForEachWidget([&](UWidget* Widget)
	{
		if (UToroSettingRowBase* Row = Cast<UToroSettingRowBase>(Widget))
		{
			Row->OnRowHovered.BindUObject(this, &UToroSettingsWidget::OnRowHovered);
		}
	});
}
