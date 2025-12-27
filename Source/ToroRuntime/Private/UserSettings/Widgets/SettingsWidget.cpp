// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserSettings/Widgets/SettingsWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "UserInterface/NativeContainers.h"
#include "Interfaces/ExitInterface.h"

USettingsWidget::USettingsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
}

void USettingsWidget::ShowWidget(UUserWidget* InParent)
{
	ParentUI = InParent;
	PushWidget();
}

// void USettingsWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
// {
// 	if (FromWidget == Difficulty)
// 	{
// 		
// 	}
// }

void USettingsWidget::OnExit()
{
	PopWidget();
}

void USettingsWidget::OnDifficulty()
{
	if (Difficulty) Difficulty->ShowWidget(this);
}

void USettingsWidget::OnAutoAdjust()
{
	if (Settings)
	{
		Settings->AutoAdjustScalability();
		AutoAdjustButton->SetIsEnabled(false);
	}
}

void USettingsWidget::OnScreenSelect(const int32 Idx)
{
	if (Idx < 0 || Idx >= ScreenSwitch->GetNumWidgets()) return;
	if (ScreenSwitch->GetActiveWidgetIndex() != Idx)
	{
		ScreenSwitch->SetActiveWidgetIndex(Idx);
		PlayAnimation(FadeAnim);
	}
}

void USettingsWidget::PopWidget()
{
	Settings->ApplySettings(false);
	IExitInterface::ReturnToWidget(ParentUI, this);
	ParentUI = nullptr;
	Super::PopWidget();
}

void USettingsWidget::PushWidget()
{
	Super::PushWidget();
	OnScreenSelect(0);
	AutoAdjustButton->SetIsEnabled(true);
}

void USettingsWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	Settings = UToroUserSettings::Get();
	ExitButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnExit);
	DifficultyButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnDifficulty);
	AutoAdjustButton->OnClicked.AddUniqueDynamic(this, &USettingsWidget::OnAutoAdjust);
	ButtonList->OnSelection.AddUObject(this, &USettingsWidget::OnScreenSelect);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Difficulty = AToroWidgetManager::GetWidget<UDifficultyWidget>(this);
	});
}
