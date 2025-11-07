// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameInputConfig.generated.h"

UENUM(BlueprintType)
enum class EGameInputMode : uint8
{
	GameAndUI,
	GameOnly,

	// Discouraged
	UI_Only
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FGameInputConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = InputConfig)
		EGameInputMode InputMode;

	UPROPERTY(EditAnywhere, Category = InputConfig, meta = (EditCondition = "InputMode != EGameInputMode::GameOnly", EditConditionHides))
		bool bShowMouseCursor;

	UPROPERTY(EditAnywhere, Category = InputConfig, meta = (EditCondition = "InputMode != EGameInputMode::GameOnly", EditConditionHides))
		EMouseLockMode MouseLock;
	
	UPROPERTY(EditAnywhere, Category = InputConfig, meta = (EditCondition = "InputMode == EGameInputMode::GameAndUI", EditConditionHides))
		bool bHideCursorOnCapture;

	UPROPERTY(VisibleAnywhere, Category = InputConfig, meta = (DisplayThumbnail = false))
		TWeakObjectPtr<UUserWidget> FocusWidget;

private:

	FInputModeGameAndUI GameAndUI;
	FInputModeGameOnly GameOnly;
	FInputModeUIOnly UI_Only;

public:
	
	FGameInputConfig()
		: InputMode(EGameInputMode::GameOnly)
		, bShowMouseCursor(false)
		, MouseLock(EMouseLockMode::LockAlways)
		, bHideCursorOnCapture(true)
		, FocusWidget(nullptr)
	{}

	FGameInputConfig(const EGameInputMode InInputMode, const bool bInShowMouseCursor,
		const EMouseLockMode InMouseLock = EMouseLockMode::LockAlways,
		const bool bInHideCursorOnCapture = true, UUserWidget* InFocusWidget = nullptr)
		: InputMode(InInputMode)
		, bShowMouseCursor(bInShowMouseCursor)
		, MouseLock(InMouseLock)
		, bHideCursorOnCapture(bInHideCursorOnCapture)
		, FocusWidget(InFocusWidget)
	{}

	void ClearAndReset()
	{
		UI_Only = FInputModeUIOnly();
		GameAndUI = FInputModeGameAndUI();
		FocusWidget = nullptr;
	}

	const FInputModeDataBase& GetInputMode()
	{
		switch (InputMode)
		{
		case EGameInputMode::GameAndUI:
			GameAndUI.SetLockMouseToViewportBehavior(MouseLock);
			GameAndUI.SetHideCursorDuringCapture(bHideCursorOnCapture);
			if (FocusWidget.IsValid()) GameAndUI.SetWidgetToFocus(FocusWidget->TakeWidget());
			return GameAndUI;

		case EGameInputMode::GameOnly:
			return GameOnly;

		case EGameInputMode::UI_Only:
			UI_Only.SetLockMouseToViewportBehavior(MouseLock);
			if (FocusWidget.IsValid()) UI_Only.SetWidgetToFocus(FocusWidget->TakeWidget());
			return UI_Only;
		}

		return GameOnly;
	}

	bool ShouldShowMouse() const { return InputMode != EGameInputMode::GameOnly && bShowMouseCursor; }
};