// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "Interfaces/ExitInterface.h"
#include "DataTypes/GameInputConfig.h"
#include "UserInterface/ToroManagedWidget.h"
#include "UserSettings/Widgets/SettingsWidget.h"
#include "GamePauseWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UGamePauseWidget final : public UToroManagedWidget, public IExitInterface
{
	GENERATED_BODY()

public:

	UGamePauseWidget(const FObjectInitializer& ObjectInitializer);

	virtual void ReturnToWidget_Implementation(UUserWidget* FromWidget) override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> ResumeButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> SettingsButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> CheckpointButton;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UButton> MainMenuButton;

	FGameInputConfig InputConfig;
	TObjectPtr<USettingsWidget> Settings;

	UFUNCTION() void OnResumeButton();
	UFUNCTION() void OnSettingsButton();
	UFUNCTION() void OnCheckpointButton();
	UFUNCTION() void OnMainMenuButton();
	void OnPauseState(const bool bPaused);
	
	virtual void PopWidget() override;
	virtual void PushWidget() override;
	virtual void InitWidget(APlayerController* Controller) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
