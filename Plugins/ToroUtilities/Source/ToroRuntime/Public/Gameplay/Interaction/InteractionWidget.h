// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroRuntime.h"
#include "InteractionData.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanelSlot.h"
#include "UserInterface/ToroManagedWidget.h"
#include "InteractionWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UInteractionWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UInteractionWidget(const FObjectInitializer& ObjectInitializer);

	void SetInteractionInfo(const FInteractionInfo& Info) const;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UPanelWidget> InteractPanel;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UWidgetSwitcher> IconSwitch;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> InteractText;

	TObjectPtr<UCanvasPanelSlot> CrosshairSlot;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool CanCreateWidget(const UObject* ContextObject) const override
	{
		return !UToroSettings::Get()->IsOnMap(ContextObject, EToroMapType::MainMenu);
	}
};
