// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserSettings/Widgets/ResolutionWidget.h"

void UResolutionWidget::ShowWidget(const TFunction<void()>& OnRevert)
{
	RevertFunc = OnRevert;
	PushWidget();
}

void UResolutionWidget::OnRevert()
{
	if (RevertFunc) RevertFunc();
	PopWidget();
}

void UResolutionWidget::PopWidget()
{
	RevertFunc = nullptr;
	Super::PopWidget();
}

void UResolutionWidget::PushWidget()
{
	Time = 5.0f;
	SetVisibility(ESlateVisibility::Visible);
	Super::PushWidget();
}

void UResolutionWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	AcceptButton->OnClicked.AddUniqueDynamic(this, &UResolutionWidget::OnAccept);
	RevertButton->OnClicked.AddUniqueDynamic(this, &UResolutionWidget::OnRevert);
}

void UResolutionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bIsPushed)
	{
		Time -= InDeltaTime;
		OnTimeUpdate(FMath::CeilToInt32(Time));
		if (Time <= 0.0f)
		{
			SetVisibility(ESlateVisibility::HitTestInvisible);
			OnRevert();
		}
	}
}
