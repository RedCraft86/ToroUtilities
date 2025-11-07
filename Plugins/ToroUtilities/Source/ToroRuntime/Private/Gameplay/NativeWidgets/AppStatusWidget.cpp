// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/NativeWidgets/AppStatusWidget.h"
#include "Framework/ToroPlayerController.h"
#include "SaveSystem/ToroSaveManager.h"
#include "Components/PanelWidget.h"

UAppStatusWidget::UAppStatusWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), TargetFPS(60.0f), UpdateTick(0.0f)
{
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UAppStatusWidget::UpdateFrameRate() const
{
	const int32 FPS = UToroUserSettings::GetAverageFPS();
	const FLinearColor Color = FMath::Lerp(FLinearColor::Red, FLinearColor::Green,
		FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, TargetFPS - 10.0f),
		FVector2D(0.0f, 1.0f), FPS));
	
	FrameRateText->SetColorAndOpacity(Color);
	FrameRateText->SetText(FText::Format(INVTEXT("{0} FPS"), FPS));

	DeltaTimeText->SetColorAndOpacity(Color);
	DeltaTimeText->SetText(FText::FromString(FString::Printf(
		TEXT("%.2f ms"), UToroUserSettings::GetAverageMS())));
}

void UAppStatusWidget::OnGameFocusChanged(const bool bFocused) const
{
	UnfocusedPanel->SetVisibility(bFocused
		? ESlateVisibility::Collapsed
		: ESlateVisibility::HitTestInvisible);
}

void UAppStatusWidget::OnSettingsChanged(const ESettingApplyType Type)
{
	if (Type == ESettingApplyType::Dynamic)
	{
		const UToroUserSettings* UserSettings = UToroUserSettings::Get();
		if (!UserSettings)
		{
			FrameRateText->SetVisibility(ESlateVisibility::Collapsed);
			DeltaTimeText->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
		
		TargetFPS = UserSettings->GetFrameRateLimit();
		if (TargetFPS > 150.0f || TargetFPS < 10.0f)
		{
			TargetFPS = 60.0f;
		}

		const bool bShowFPS = UserSettings->GetShowFPS();
		FrameRateText->SetVisibility(bShowFPS ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		DeltaTimeText->SetVisibility(bShowFPS ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

		UpdateFrameRate();
	}
}

void UAppStatusWidget::OnSaveLoad(const UToroSaveObject* SaveObject, const ESaveGameActivity Activity)
{
	if (SaveObject && Activity == ESaveGameActivity::Saving)
	{
		PlayAnimation(SaveAnim, 0.0f, 5);
	}
}

void UAppStatusWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	UnfocusedPanel->SetVisibility(ESlateVisibility::Collapsed);

	OnSettingsChanged(ESettingApplyType::Dynamic);
	if (UToroUserSettings* Settings = UToroUserSettings::Get())
	{
		Settings->OnSettingsUpdated.AddUObject(this, &UAppStatusWidget::OnSettingsChanged);
	}

	if (AToroPlayerController* PC = AToroPlayerController::Get(this))
	{
		PC->OnGameFocusChanged.AddUObject(this, &UAppStatusWidget::OnGameFocusChanged);
	}

	if (UToroSaveManager* SaveManager = UToroSaveManager::Get(this))
	{
		SaveManager->OnSaveActivity.AddUObject(this, &UAppStatusWidget::OnSaveLoad);
	}

	AddToViewport(200);
}

void UAppStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (FrameRateText->GetVisibility() == ESlateVisibility::Collapsed) return;
	if (UpdateTick >= 0.1f)
	{
		UpdateTick = 0.0f;
		UpdateFrameRate();
	}
	else
	{
		UpdateTick += InDeltaTime;
	}
}
