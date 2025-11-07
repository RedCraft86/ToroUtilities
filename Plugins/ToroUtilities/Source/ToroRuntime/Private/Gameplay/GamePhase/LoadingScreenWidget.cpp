// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/GamePhase/LoadingScreenWidget.h"
#include "UserInterface/NativeContainers.h"

ULoadingScreenWidget::ULoadingScreenWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), bMinimal(false), MaxPackages(0)
{
	bAutoPush = false;
	ContainerClass = UOverlayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

FText ULoadingScreenWidget::FormatProgress_Implementation(const FIntPoint& Progress)
{
	return FText::Format(INVTEXT("Loading: {0}/{1} Packages"), Progress.X, Progress.Y);
}

void ULoadingScreenWidget::ShowWidget(const bool bMinimalUI)
{
	if (!IsPushed())
	{
		bMinimal = bMinimalUI;
		PushWidget();
	}
}

void ULoadingScreenWidget::PushWidget()
{
	Super::PushWidget();
	if (IsPushed())
	{
		MaxPackages = 0;
		ScreenSwitch->SetActiveWidgetIndex(bMinimal ? 1 : 0);
	}
}

void ULoadingScreenWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bMinimal && IsPushed())
	{
		const int32 Packages = GetNumAsyncPackages();
		if (Packages > MaxPackages) MaxPackages = Packages;
		LoadProgress->SetPercent((float)Packages / MaxPackages);
		LoadMessage->SetText(FormatProgress(FIntPoint(Packages, MaxPackages)));
	}
}
