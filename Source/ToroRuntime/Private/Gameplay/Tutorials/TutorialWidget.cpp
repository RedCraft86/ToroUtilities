// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/Tutorials/TutorialWidget.h"

#include "Animation/WidgetAnimation.h"
#include "UserInterface/NativeContainers.h"
#include "Framework/ToroPlayerController.h"
#include "Components/ScaleBox.h"

void UTutorialEntryWidget::InitializeWidget(const FTutorialEntry& Entry) const
{
	TitleText->SetText(Entry.Name);
	ContentText->SetText(Entry.Description);
	if (UTexture2D* Image = Entry.Image.LoadSynchronous())
	{
		PreviewImage->SetBrushFromTexture(Image);
	}
	else if (UScaleBox* Scale = Cast<UScaleBox>(PreviewImage->GetParent()))
	{
		Scale->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UTutorialWidget::UTutorialWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UOverlayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
}

void UTutorialWidget::ShowWidget(const FTutorialEntry& Entry)
{
	CreateEntry(Entry);
	if (!IsPushed())
	{
		PushWidget();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UTutorialWidget::MoveLeft()
{
	const int32 NextIdx = EntryContainer->GetActiveWidgetIndex() - 1;
	if (NextIdx > 0)
	{
		EntryContainer->SetActiveWidgetIndex(NextIdx);
		UpdatePageCount();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UTutorialWidget::MoveRight()
{
	const int32 NextIdx = EntryContainer->GetActiveWidgetIndex() + 1;
	if (NextIdx < EntryContainer->GetNumWidgets())
	{
		EntryContainer->SetActiveWidgetIndex(NextIdx);
		UpdatePageCount();
	}
}

void UTutorialWidget::UpdatePageCount() const
{
	const uint8 Current = EntryContainer->GetActiveWidgetIndex() + 1, Max = EntryContainer->GetNumWidgets();
	if (Max > 1) PageCount->SetText(FText::Format(INVTEXT("{0}/{1}"), Current, Max));
	
	PageBox->SetVisibility(Max == 1 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	if (Max == 1 || Current == Max)
	{
		CloseButton->SetVisibility(ESlateVisibility::Visible);
	}
}

void UTutorialWidget::CreateEntry(const FTutorialEntry& Entry)
{
	if (UTutorialEntryWidget* Widget = CreateWidget<UTutorialEntryWidget>(this, EntryClass))
	{
		EntryContainer->AddChild(Widget);
		Widget->InitializeWidget(Entry);
		UpdatePageCount();
	}
}

void UTutorialWidget::PushWidget()
{
	Super::PushWidget();
	PlayAnimation(ShowAnim);

	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, false});
		PC->AddPauseRequest(this);
	}
}

void UTutorialWidget::PopWidget()
{
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputConfig(InputConfig);
		PC->RemovePauseRequest(this);
	}
	if (CloseFunc) CloseFunc();
	EntryContainer->ClearChildren();
	CloseButton->SetVisibility(ESlateVisibility::Collapsed);
	Super::PopWidget();
}

void UTutorialWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	CloseButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::CloseMenu);
	LeftButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::MoveLeft);
	RightButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::MoveRight);
}
