// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

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