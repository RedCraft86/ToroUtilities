// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UserInterface/ToroManagedWidget.h"
#include "UserSettings/ToroUserSettings.h"
#include "DifficultyWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UDifficultyWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

	static inline TArray<FText> Options = {
		INVTEXT("Easy"), INVTEXT("Normal"), INVTEXT("Hard")
	};

public:

	UDifficultyWidget(const FObjectInitializer& ObjectInitializer);

	void ShowWidget(UUserWidget* InParent);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> LeftButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> RightButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> DescSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	uint8 Difficulty;
	TObjectPtr<UUserWidget> ParentUI;
	TObjectPtr<UToroUserSettings> Settings;

	UFUNCTION() void OnLeftButton();
	UFUNCTION() void OnRightButton();
	UFUNCTION() void OnExitButton();
	void SyncVisualState() const;

	virtual void PopWidget() override;
	virtual void PushWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
};
