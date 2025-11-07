// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Cutscenes/CutsceneWidget.h"
#include "Gameplay/Cutscenes/ToroCutsceneActor.h"
#include "UserInterface/NativeContainers.h"

UCutsceneWidget::UCutsceneWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), SkipKey(EKeys::Enter), HoldTime(2.0f), bHolding(false), Progress(0.0f)
{
	ContainerClass = UOverlayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UCutsceneWidget::ShowWidget(AToroCutsceneActor* InCutscene)
{
	Cutscene = InCutscene;
	Progress = 0.0f;
	PushWidget();
}

void UCutsceneWidget::PopWidget()
{
	Cutscene.Reset();
	SetHoldingState(false);
	Super::PopWidget();
}

void UCutsceneWidget::SkipCutscene() const
{
	if (Cutscene.IsValid()) Cutscene->SkipCutscene();
}

void UCutsceneWidget::SetHoldingState(const bool InState)
{
	if (bHolding != InState)
	{
		bHolding = InState;
		PlayAnimation(HoldAnim, 0.0f, 1,
			bHolding ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
	}
}

void UCutsceneWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (SkipKeyText) SkipKeyText->SetText(SkipKey.GetDisplayName());
}

void UCutsceneWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bHolding && Progress < 1.0f)
	{
		Progress += InDeltaTime / HoldTime;
		if (Progress >= 1.0f)
		{
			Progress = 1.0f;
			SkipCutscene();
		}
	}
	else if (!bHolding && Progress > 0.0f)
	{
		Progress = FMath::Max(Progress - (InDeltaTime * HoldTime), 0.0f);
	}

	SkipProgressBar->SetPercent(Progress);
}

FReply UCutsceneWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (Cutscene.IsValid() && InKeyEvent.GetKey() == SkipKey)
	{
		SetHoldingState(true);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

FReply UCutsceneWidget::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == SkipKey)
	{
		SetHoldingState(false);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
