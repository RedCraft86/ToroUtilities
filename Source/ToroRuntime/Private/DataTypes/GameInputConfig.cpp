// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "DataTypes/GameInputConfig.h"

const FGameInputConfig FGameInputConfig::Default = FGameInputConfig();

void FGameInputConfig::Reset()
{
	UI_Only = FInputModeUIOnly();
	GameAndUI = FInputModeGameAndUI();
	FocusWidget.Reset();
}

bool FGameInputConfig::ShouldShowMouse() const
{
	return InputMode != EGameInputMode::GameOnly && bShowMouseCursor;
}

const FInputModeDataBase& FGameInputConfig::GetInputMode()
{
	switch (InputMode)
	{
	case EGameInputMode::GameAndUI:
		GameAndUI.SetLockMouseToViewportBehavior(MouseLock);
		GameAndUI.SetHideCursorDuringCapture(bHideCursorOnCapture);
		if (FocusWidget.IsValid()) GameAndUI.SetWidgetToFocus(FocusWidget->TakeWidget());
		return GameAndUI;

	case EGameInputMode::UI_Only:
		UI_Only.SetLockMouseToViewportBehavior(MouseLock);
		if (FocusWidget.IsValid()) UI_Only.SetWidgetToFocus(FocusWidget->TakeWidget());
		return UI_Only;

	default: 
		return GameOnly;
	}
}