// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameInputConfig.generated.h"

/**
 * Defines the high-level input routing for the application.
 */
UENUM(BlueprintType)
enum class EGameInputMode : uint8
{
	/** Allows both Game (PlayerController) and UI (Widgets) to receive input. */
	GameAndUI,

	/** Restricts input strictly to the game world. UI will not capture mouse or keyboard. */
	GameOnly,

	/** Restricts input strictly to UI. The game world/PlayerController will ignore most inputs. */
	UI_Only
};

/**
 * A comprehensive configuration struct used to define how the Mouse, Keyboard, and Viewport 
 * behave during different game states (e.g., Menu open, Gameplay, Cutscene).
 */
USTRUCT(BlueprintType)
struct TORORUNTIME_API FGameInputConfig final
{
	GENERATED_BODY()

	/** A default configuration set to GameOnly with cursor hidden. */
	static const FGameInputConfig Default;

	/** The routing mode for input (Game, UI, or both). */
	UPROPERTY(EditAnywhere, Category = InputConfig)
		EGameInputMode InputMode;

	/** Whether the software/hardware mouse cursor should be visible on screen. */
	UPROPERTY(EditAnywhere, Category = InputConfig, meta = (EditCondition = "InputMode != EGameInputMode::GameOnly", EditConditionHides))
		bool bShowMouseCursor;

	/** How the mouse cursor should be constrained to the viewport. */
	UPROPERTY(EditAnywhere, Category = InputConfig, meta = (EditCondition = "InputMode != EGameInputMode::GameOnly", EditConditionHides))
		EMouseLockMode MouseLock;

	/** If true, the cursor is hidden automatically when the mouse is "captured" (clicked into the viewport). */
	UPROPERTY(EditAnywhere, Category = InputConfig, meta = (EditCondition = "InputMode == EGameInputMode::GameAndUI", EditConditionHides))
		bool bHideCursorOnCapture;

	/** 
	 * The specific widget that should receive keyboard/gamepad focus when this config is applied. 
	 * Uses a Weak Pointer to prevent memory leaks if the widget is destroyed.
	 */
	UPROPERTY(VisibleAnywhere, Category = InputConfig, meta = (DisplayThumbnail = false))
		TWeakObjectPtr<UUserWidget> FocusWidget;

	FGameInputConfig()
		: InputMode(EGameInputMode::GameOnly)
		, bShowMouseCursor(false)
		, MouseLock(EMouseLockMode::LockAlways)
		, bHideCursorOnCapture(true)
		, FocusWidget(nullptr)
	{}

	FGameInputConfig(const EGameInputMode InInputMode, const bool bInShowMouseCursor = false, 
		const EMouseLockMode InMouseLock = EMouseLockMode::LockAlways,
		const bool bInHideCursorOnCapture = true, UUserWidget* InFocusWidget = nullptr)
		: InputMode(InInputMode)
		, bShowMouseCursor(bInShowMouseCursor)
		, MouseLock(InMouseLock)
		, bHideCursorOnCapture(bInHideCursorOnCapture)
		, FocusWidget(InFocusWidget)
	{}

	void Reset();
	bool ShouldShowMouse() const;
	const FInputModeDataBase& GetInputMode();

private:

	FInputModeGameAndUI GameAndUI;
	FInputModeGameOnly GameOnly;
	FInputModeUIOnly UI_Only;
};