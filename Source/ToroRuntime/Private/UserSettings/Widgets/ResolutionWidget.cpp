// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserSettings/Widgets/ResolutionWidget.h"
#include "UserInterface/NativeContainers.h"

UResolutionWidget::UResolutionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), Time(5.0f)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::Visible);
}

void UResolutionWidget::ShowWidget(const TFunction<void()>& OnRevertFunc, const TFunction<void()>& OnAcceptFunc)
{
	RevertFunc = OnRevertFunc;
	AcceptFunc = OnAcceptFunc;
	PushWidget();
}

void UResolutionWidget::OnRevert()
{
	if (RevertFunc) RevertFunc();
	PopWidget();
}

void UResolutionWidget::OnAccept()
{
	if (AcceptFunc) AcceptFunc();
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
	RevertButton->OnClicked.AddUniqueDynamic(this, &UResolutionWidget::OnRevert);
	AcceptButton->OnClicked.AddUniqueDynamic(this, &UResolutionWidget::OnAccept);
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
