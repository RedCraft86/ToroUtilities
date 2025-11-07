// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/NativeWidgets/GamePauseWidget.h"
#include "UserInterface/ToroWidgetManager.h"
#include "UserInterface/NativeContainers.h"
#include "Framework/ToroPlayerController.h"
#include "Libraries/ToroShortcutLibrary.h"
#include "EnhancedCodeFlow.h"
#include "ToroRuntime.h"

UGamePauseWidget::UGamePauseWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
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
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetUserPause(false);
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
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);
	FFlow::Delay(this, 0.6f, [this]()
	{
		UToroShortcutLibrary::RestartLevel(this);
	});
}

void UGamePauseWidget::OnMainMenuButton()
{
	SetHidden(true);
	SetVisibility(ESlateVisibility::HitTestInvisible);
	UToroShortcutLibrary::StartCameraFade(this, 0.0f, 1.0f, 0.5f);
	FFlow::Delay(this, 0.6f, [this]()
	{
		if (const TSoftObjectPtr<UWorld>* Map = UToroSettings::Get()->MapRegistry.Find(EToroMapType::MainMenu))
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, *Map);
		}
	});
}

void UGamePauseWidget::OnPauseState(const bool bPaused)
{
	bPaused ? PushWidget() : PopWidget();
}

void UGamePauseWidget::PopWidget()
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig(InputConfig);
	}
	Super::PopWidget();
}

void UGamePauseWidget::PushWidget()
{
	Super::PushWidget();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		InputConfig = PC->GetInputConfig();
		PC->SetInputConfig({EGameInputMode::GameAndUI, true,
			EMouseLockMode::LockAlways, false, this});
	}
}

void UGamePauseWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		Settings = AToroWidgetManager::GetWidget<USettingsWidget>(this);
		if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
		{
			PC->OnUserPaused.AddUObject(this, &UGamePauseWidget::OnPauseState);
		}
	});
}
