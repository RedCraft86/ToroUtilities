// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/NativeWidgets/SubtitleWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "UserInterface/NativeContainers.h"
// #include "SubtitleManager.h"

USubtitleWidget::USubtitleWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), NameColor(FLinearColor::White), PlayerColor(FLinearColor::Yellow)
{
	ContainerClass = UGameplayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void USubtitleWidget::QueueSubtitle(const UObject* ContextObject, const FToroSubtitle& InSubtitle)
{
	if (USubtitleWidget* Widget = AToroWidgetManager::GetWidget<USubtitleWidget>(ContextObject))
	{
		Widget->AddSubtitle(InSubtitle);
	}
}

void USubtitleWidget::QueueSubtitles(const UObject* ContextObject, const TArray<FToroSubtitle> InSubtitles)
{
	if (USubtitleWidget* Widget = AToroWidgetManager::GetWidget<USubtitleWidget>(ContextObject))
	{
		Widget->AddSubtitles(InSubtitles);
	}
}

void USubtitleWidget::NextSubtitle()
{
	if (!Subtitles.IsEmpty())
	{
		ShowSubtitle(Subtitles[0]);
		Subtitles.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(SubtitleAnim);
	}
}

void USubtitleWidget::ShowSubtitle(const FToroSubtitle& InData)
{
	LineText->SetText(InData.Line);
	NameText->SetColorAndOpacity(InData.IsPlayer() ? PlayerColor : NameColor);
	NameText->SetText(InData.Name.IsEmptyOrWhitespace() ? FText::GetEmpty()
		: FText::Format(INVTEXT("{0}:"), InData.Name));
	PlayAnimation(SubtitleAnim);
	GetWorld()->GetTimerManager().SetTimer(SubtitleTimer, this,
		&USubtitleWidget::NextSubtitle, InData.CalcTime(), false);
}

void USubtitleWidget::AddSubtitle(const FToroSubtitle& InSubtitle)
{
	if (!InSubtitle.IsValidData()) return;
	if (!Subtitles.IsEmpty() && Subtitles.Last().Matches(InSubtitle)) return;

	Subtitles.Add(InSubtitle);
	if (!GetWorld()->GetTimerManager().IsTimerActive(SubtitleTimer))
	{
		NextSubtitle();
	}
}

void USubtitleWidget::AddSubtitles(const TArray<FToroSubtitle>& InSubtitles)
{
	for (const FToroSubtitle& Subtitle : InSubtitles)
	{
		AddSubtitle(Subtitle);
	}
}

void USubtitleWidget::OnNativeSubtitle(const FText& InText)
{
	GetWorld()->GetTimerManager().ClearTimer(SubtitleTimer);
	ShowSubtitle({FText::GetEmpty(), InText, 0.5f});
}

void USubtitleWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	LineText->SetText(FText::GetEmpty());
	NameText->SetText(FText::GetEmpty());
	// if (bCaptureNativeSubtitles)
	// {
	// 	FSubtitleManager::GetSubtitleManager()->OnSetSubtitleText()
	// 		.AddUObject(this, &USubtitleWidget::OnNativeSubtitle);
	// }
}
