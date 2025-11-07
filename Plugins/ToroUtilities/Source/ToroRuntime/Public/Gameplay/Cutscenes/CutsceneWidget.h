// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "UserInterface/ToroManagedWidget.h"
#include "CutsceneWidget.generated.h"

UCLASS(Abstract)
class TORORUNTIME_API UCutsceneWidget final : public UToroManagedWidget
{
	GENERATED_BODY()

public:

	UCutsceneWidget(const FObjectInitializer& ObjectInitializer);

	void ShowWidget(class AToroCutsceneActor* InCutscene);
	virtual void PopWidget() override;

protected:

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UTextBlock> SkipKeyText;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Elements, meta = (BindWidget))
		TObjectPtr<UProgressBar> SkipProgressBar;

	UPROPERTY(Transient, BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim))
		TObjectPtr<UWidgetAnimation> HoldAnim;

	UPROPERTY(EditAnywhere, Category = Settings)
		FKey SkipKey;

	UPROPERTY(EditAnywhere, Category = Settings, meta = (ClampMin = 0.5f, UIMin = 0.5f))
		float HoldTime;

	bool bHolding;
	float Progress;
	TWeakObjectPtr<AToroCutsceneActor> Cutscene;

	void SkipCutscene() const;
	void SetHoldingState(const bool InState);

	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
