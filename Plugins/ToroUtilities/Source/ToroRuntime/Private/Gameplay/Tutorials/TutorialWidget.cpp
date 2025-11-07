// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Tutorials/TutorialWidget.h"
#include "UserInterface/NativeContainers.h"
#include "Framework/ToroPlayerController.h"

void UTutorialEntryWidget::InitializeWidget(const FTutorialEntry& Entry) const
{
	PreviewImage->SetBrushFromTexture(Entry.Image.LoadSynchronous());
	ContentText->SetText(Entry.Description);
	TitleText->SetText(Entry.Name);
}

UTutorialWidget::UTutorialWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UOverlayWidgetContainer::StaticClass();
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
	PageCount->SetText(FText::Format(INVTEXT("{0}/{1}"),
		EntryContainer->GetActiveWidgetIndex() + 1,
		EntryContainer->GetNumWidgets()));
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
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, this});
		PC->AddPauseRequest(this);
	}
}

void UTutorialWidget::PopWidget()
{
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig(InputConfig);
		PC->RemovePauseRequest(this);
	}
	if (CloseFunc) CloseFunc();
	EntryContainer->ClearChildren();
	Super::PopWidget();
}

void UTutorialWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	CloseButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::CloseMenu);
	LeftButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::MoveLeft);
	RightButton->OnClicked.AddUniqueDynamic(this, &UTutorialWidget::MoveRight);
}
