// Copyright (C) RedCraft86. All Rights Reserved.

#include "Gameplay/Interaction/InteractionWidget.h"
#include "UserInterface/NativeContainers.h"
#include "Components/CanvasPanelSlot.h"

UInteractionWidget::UInteractionWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bAutoPush = false;
	ContainerClass = UGameplayWidgetContainer::StaticClass();
	UUserWidget::SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UInteractionWidget::SetInteractionInfo(const FInteractionInfo& Info) const
{
	IconSwitch->SetActiveWidgetIndex(Info.bEnabled ? 1 : 0);
	InteractText->SetText(Info.Label);
	InteractText->SetVisibility(Info.bEnabled
		? ESlateVisibility::HitTestInvisible
		: ESlateVisibility::Visible);
}

void UInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CrosshairSlot = Cast<UCanvasPanelSlot>(InteractPanel->Slot);
}

void UInteractionWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (const APlayerController* PC = GetOwningPlayer())
	{
		if (PC->ShouldShowMouseCursor())
		{
			// Hide crosshair since there is mouse pointer
			IconSwitch->SetVisibility(ESlateVisibility::Collapsed);

			FVector2D MousePos;
			PC->GetMousePosition(MousePos.X, MousePos.Y);
			if (CrosshairSlot) CrosshairSlot->SetPosition(MyGeometry.AbsoluteToLocal(MousePos));
		}
		else
		{
			IconSwitch->SetVisibility(ESlateVisibility::HitTestInvisible);
			if (CrosshairSlot) CrosshairSlot->SetPosition(FVector2D::ZeroVector);
		}
	}
}
