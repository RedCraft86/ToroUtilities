// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/NativeWidgets/NoticeWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "UserInterface/NativeContainers.h"
#include "Components/HorizontalBoxSlot.h"
#include "Helpers/WidgetAnimHelpers.h"
#include "Blueprint/WidgetTree.h"
#include "EnhancedCodeFlow.h"

void UInputPreviewEntry::InitializeWidget(const FToroInputPrompt& InPreview)
{
	KeysBox->ClearChildren();
	NameText->SetText(InPreview.Name);
	for (const FKey& Key : InPreview.Keys)
	{
		FRichImageRow Icon = GetKeyIcon(Key.GetFName());
		if (!Icon.Brush.GetResourceObject()) continue;
		if (UImage* Image = WidgetTree->ConstructWidget<UImage>())
		{
			Image->SetBrush(Icon.Brush);
			Image->SetDesiredSizeOverride(Icon.Brush.GetImageSize() * 0.75f);
			if (UHorizontalBoxSlot* BoxSlot = KeysBox->AddChildToHorizontalBox(Image))
			{
				BoxSlot->SetVerticalAlignment(VAlign_Center);
				BoxSlot->SetHorizontalAlignment(HAlign_Center);
				BoxSlot->SetPadding(FMargin(2.0f, 0.0f));
			}
		}
	}
}

UNoticeWidget::UNoticeWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ContainerClass = UOverlayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UNoticeWidget::QueueNotice(const UObject* ContextObject, const FToroSimpleMsg& InNotice, const bool bImmediate)
{
	if (UNoticeWidget* Widget = AToroWidgetManager::GetWidget<UNoticeWidget>(ContextObject))
	{
		Widget->AddNotice(InNotice, bImmediate);
	}
}

void UNoticeWidget::QueueTitle(const UObject* ContextObject, const FToroSimpleMsg& InTitle, const bool bImmediate)
{
	if (UNoticeWidget* Widget = AToroWidgetManager::GetWidget<UNoticeWidget>(ContextObject))
	{
		Widget->AddTitle(InTitle, bImmediate);
	}
}

void UNoticeWidget::ShowInputPrompt(const UObject* ContextObject, const FName InKey, const FToroInputPrompt& InData)
{
	if (UNoticeWidget* Widget = AToroWidgetManager::GetWidget<UNoticeWidget>(ContextObject))
	{
		Widget->AddInputRow(InKey, InData);
	}
}

void UNoticeWidget::HideInputPrompt(const UObject* ContextObject, const FName InKey)
{
	if (UNoticeWidget* Widget = AToroWidgetManager::GetWidget<UNoticeWidget>(ContextObject))
	{
		Widget->RemoveInputRow(InKey);
	}
}

void UNoticeWidget::ClearInputPrompts(const UObject* ContextObject)
{
	if (UNoticeWidget* Widget = AToroWidgetManager::GetWidget<UNoticeWidget>(ContextObject))
	{
		Widget->ClearInputRows();
	}
}

void UNoticeWidget::ShowNotice(const FToroSimpleMsg& InData)
{
	NoticeText->SetText(InData.Message);
	PlayAnimation(NoticeAnim);
	GetWorld()->GetTimerManager().SetTimer(NoticeTimer, this,
		&UNoticeWidget::NextNotice, InData.CalcTime(), false);
}

void UNoticeWidget::AddNotice(const FToroSimpleMsg& InNotice, const bool bImmediate)
{
	if (!InNotice.IsValidData()) return;
	if (!Notices.IsEmpty() && Notices.Last().Matches(InNotice)) return;
	if (bImmediate)
	{
		GetWorld()->GetTimerManager().ClearTimer(NoticeTimer);
		ShowNotice(InNotice);
	}
	else
	{
		Notices.Add(InNotice);
		if (!GetWorld()->GetTimerManager().IsTimerActive(NoticeTimer))
		{
			NextNotice();
		}
	}
}

void UNoticeWidget::NextNotice()
{
	if (!Notices.IsEmpty())
	{
		ShowNotice(Notices[0]);
		Notices.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(NoticeAnim);
	}
}

void UNoticeWidget::ShowTitle(const FToroSimpleMsg& InData)
{
	TitleText->SetText(InData.Message);
	PlayAnimation(TitleAnim);
	GetWorld()->GetTimerManager().SetTimer(TitleTimer, this,
		&UNoticeWidget::NextTitle, InData.CalcTime(), false);
}

void UNoticeWidget::AddTitle(const FToroSimpleMsg& InTitle, const bool bImmediate)
{
	if (!InTitle.IsValidData()) return;
	if (!Titles.IsEmpty() && Titles.Last().Matches(InTitle)) return;
	if (bImmediate)
	{
		GetWorld()->GetTimerManager().ClearTimer(TitleTimer);
		ShowTitle(InTitle);
	}
	else
	{
		Titles.Add(InTitle);
		if (!GetWorld()->GetTimerManager().IsTimerActive(TitleTimer))
		{
			NextTitle();
		}
	}
}

void UNoticeWidget::NextTitle()
{
	if (!Titles.IsEmpty())
	{
		ShowNotice(Titles[0]);
		Titles.RemoveAt(0);
	}
	else
	{
		PlayAnimationReverse(TitleAnim);
	}
}

void UNoticeWidget::ClearInputRows()
{
	if (InputPreviews.IsEmpty()) return;

	InputPreviews.Empty();
	PlayAnimationReverse(InputRowAnim);
	const float Duration = WidgetAnimHelpers::GetAnimDuration(InputRowAnim);
	FFlow::Delay(this, Duration, [this]()
	{
		InputContainer->ClearChildren();
	});
}

void UNoticeWidget::RemoveInputRow(const FName& InKey)
{
	if (!InputPreviews.Contains(InKey)) return;

	UInputPreviewEntry* Widget = InputPreviews[InKey];
	InputPreviews.Remove(InKey);

	const bool bLastRow = InputPreviews.Num() == 1;
	if (bLastRow)
	{
		PlayAnimationReverse(InputRowAnim);
	}

	const float Duration = WidgetAnimHelpers::GetAnimDuration(InputRowAnim);
	FFlow::Delay(this, bLastRow ? Duration : 0.01f, [this, Widget]()
	{
		Widget->RemoveFromParent();
	});
}

void UNoticeWidget::AddInputRow(const FName& InKey, const FToroInputPrompt& InData)
{
	if (!InputEntryClass || InputPreviews.Contains(InKey) || !InData.IsValidData()) return;
	if (UInputPreviewEntry* Entry = CreateWidget<UInputPreviewEntry>(this, InputEntryClass))
	{
		Entry->InitializeWidget(InData);
		InputContainer->AddChild(Entry);
		InputPreviews.Add(InKey, Entry);
		PlayAnimationForward(InputRowAnim);
	}
}

void UNoticeWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	NoticeText->SetText(FText::GetEmpty());
	TitleText->SetText(FText::GetEmpty());
}
