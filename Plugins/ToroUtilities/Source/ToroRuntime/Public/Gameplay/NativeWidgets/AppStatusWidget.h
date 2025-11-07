// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/TextBlock.h"
#include "SaveSystem/ToroSaveTypes.h"
#include "UserInterface/ToroWidgetBase.h"
#include "UserSettings/ToroUserSettings.h"
#include "AppStatusWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UAppStatusWidget final : public UToroWidgetBase
{
	GENERATED_BODY()

public:

	UAppStatusWidget(const FObjectInitializer& ObjectInitializer);

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> UnfocusedPanel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> FrameRateText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> DeltaTimeText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> SaveAnim;

	float TargetFPS, UpdateTick;

	void UpdateFrameRate() const;
	void OnGameFocusChanged(const bool bFocused) const;
	void OnSettingsChanged(const ESettingApplyType Type);
	void OnSaveLoad(const UToroSaveObject* SaveObject, const ESaveGameActivity Activity);

	virtual void InitWidget(APlayerController* Controller) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
