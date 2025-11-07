// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Inspection/InspectionWidget.h"
#include "UserInterface/NativeContainers.h"
#include "Framework/ToroPlayerController.h"

UInspectionWidget::UInspectionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UMenuWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInspectionWidget::ShowWidget(const UInventoryAsset* InArchive)
{
	if (!IsPushed() && InArchive && InArchive->IsValidArchive())
	{
		ThumbnailImage->SetBrushFromTexture(InArchive->Thumbnail.LoadSynchronous());
		TitleText->SetText(InArchive->DisplayName);
		DescText->SetText(InArchive->Description);
		ContentText->SetText(InArchive->ContentText);
		PushWidget();
	}
}

void UInspectionWidget::PopWidget()
{
	if (AToroPlayerController* PC = GetOwningPlayer<AToroPlayerController>())
	{
		PC->SetInputConfig(InputConfig);
		PC->RemovePauseRequest(this);
	}
	Super::PopWidget();
}

void UInspectionWidget::PushWidget()
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
