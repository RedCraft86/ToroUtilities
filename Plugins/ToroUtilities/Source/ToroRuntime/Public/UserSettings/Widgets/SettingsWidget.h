// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Interfaces/ExitInterface.h"
#include "Components/WidgetSwitcher.h"
#include "NativeWidgets/DifficultyWidget.h"
#include "UserInterface/ToroManagedWidget.h"
#include "UserInterface/Elements/ToroButtonList.h"
#include "UserSettings/ToroUserSettings.h"
#include "SettingsWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API USettingsWidget final : public UToroManagedWidget, public IExitInterface
{
	GENERATED_BODY()

public:

	USettingsWidget(const FObjectInitializer& ObjectInitializer);

	void ShowWidget(UUserWidget* InParent);
	// virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UToroButtonList> ButtonList;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> ScreenSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> LabelText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> DifficultyButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> AutoAdjustButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ExitButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> FadeAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		TArray<FText> TextLabels;

	TObjectPtr<UUserWidget> ParentUI;
	TObjectPtr<UDifficultyWidget> Difficulty;
	TObjectPtr<UToroUserSettings> Settings;

	UFUNCTION() void OnExit();
	UFUNCTION() void OnDifficulty();
	UFUNCTION() void OnAutoAdjust();
	void OnScreenSelect(const int32 Idx);

	virtual void PopWidget() override;
	virtual void PushWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
};
