// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "GameInputConfig.generated.h"

UENUM(BlueprintType)
enum class EGameInputMode : uint8
{
	GameAndUI,
	GameOnly,
	UI_Only
};

USTRUCT(BlueprintType)
struct TORORUNTIME_API FGameInputConfig final
{
	GENERATED_BODY()

	static const FGameInputConfig Default;

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