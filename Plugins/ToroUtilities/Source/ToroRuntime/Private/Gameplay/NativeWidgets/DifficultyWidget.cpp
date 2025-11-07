// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/NativeWidgets/DifficultyWidget.h"
#include "UserInterface/NativeContainers.h"
#include "UserSettings/ToroUserSettings.h"
#include "Interfaces/ExitInterface.h"

UDifficultyWidget::UDifficultyWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Difficulty(1)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
}

void UDifficultyWidget::ShowWidget(UUserWidget* InParent)
{
	ParentUI = InParent;
	PushWidget();
}

void UDifficultyWidget::OnLeftButton()
{
	if (Difficulty < Options.Num() - 1)
	{
		Difficulty++;
		SyncVisualState();
	}
}

void UDifficultyWidget::OnRightButton()
{
	if (Difficulty > 0)
	{
		Difficulty--;
		SyncVisualState();
	}
}

void UDifficultyWidget::OnExitButton()
{
	PopWidget();
}

void UDifficultyWidget::SyncVisualState() const
{
	LabelText->SetText(Options[Difficulty]);
	DescSwitch->SetActiveWidgetIndex(Difficulty);
}

void UDifficultyWidget::PopWidget()
{
	Settings->SetDifficulty(static_cast<EGameDifficulty>(Difficulty));
	IExitInterface::ReturnToWidget(ParentUI, this);
	ParentUI = nullptr;
	Super::PopWidget();
}

void UDifficultyWidget::PushWidget()
{
	Super::PushWidget();
	Difficulty = static_cast<uint8>(Settings->GetDifficulty());
	SyncVisualState();
}

void UDifficultyWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	Settings = UToroUserSettings::Get();
	LeftButton->OnClicked.AddUniqueDynamic(this, &UDifficultyWidget::OnLeftButton);
	RightButton->OnClicked.AddUniqueDynamic(this, &UDifficultyWidget::OnRightButton);
	ExitButton->OnClicked.AddUniqueDynamic(this, &UDifficultyWidget::OnExitButton);
}
