// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "Gameplay/NativeWidgets/GamePauseWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "UserInterface/NativeContainers.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "ToroRuntime.h"

UGamePauseWidget::UGamePauseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
}

void UGamePauseWidget::ReturnToWidget_Implementation(UUserWidget* FromWidget)
{
	if (FromWidget == Settings)
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UGamePauseWidget::OnResumeButton()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetUserPause(false);
		PopWidget();
	}
}

void UGamePauseWidget::OnSettingsButton()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	Settings->ShowWidget(this);
}

void UGamePauseWidget::OnCheckpointButton()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UToroShortcutLibrary::RestartLevel(this);
}

void UGamePauseWidget::OnMainMenuButton()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (const TSoftObjectPtr<UWorld>* Map = UToroSettings::Get()->MapRegistry.Find(EToroMapType::MainMenu))
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, *Map);
	}
}

void UGamePauseWidget::OnPauseState(const bool bPaused)
{
	bPaused ? PushWidget() : PopWidget();
}

void UGamePauseWidget::PopWidget()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->SetInputConfig(InputConfig);
	}
	Super::PopWidget();
}

void UGamePauseWidget::PushWidget()
{
	Super::PushWidget();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	VersionText->SetText(UToroSettings::Get()->GetVersionLabel());
	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::UI_Only, true,
			EMouseLockMode::LockAlways, false});
	}
}

void UGamePauseWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	ResumeButton->OnClicked.AddUniqueDynamic(this, &UGamePauseWidget::OnResumeButton);
	SettingsButton->OnClicked.AddUniqueDynamic(this, &UGamePauseWidget::OnSettingsButton);
	CheckpointButton->OnClicked.AddUniqueDynamic(this, &UGamePauseWidget::OnCheckpointButton);
	MainMenuButton->OnClicked.AddUniqueDynamic(this, &UGamePauseWidget::OnMainMenuButton);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Settings = AToroWidgetManager::GetWidget<USettingsWidget>(this);
		if (AToroPlayerController* PC = AToroPlayerController::Get(this))
		{
			PC->OnUserPaused.AddUObject(this, &UGamePauseWidget::OnPauseState);
		}
	});
}
