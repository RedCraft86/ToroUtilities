// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserWidgets/ToroMasterWidget.h"

UToroMasterWidget::UToroMasterWidget(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit)
{
}

void UToroMasterWidget::PushToStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const
{
	if (MasterStack && WidgetClass)
	{
		MasterStack->AddWidget(WidgetClass);
	}
}

void UToroMasterWidget::PopFromStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const
{
	if (MasterStack && WidgetClass)
	{
		const TArray<UCommonActivatableWidget*>& List = MasterStack->GetWidgetList();
		for (int i = List.Num() - 1; i >= 0; i--)
		{
			if (List[i] && List[i]->GetClass() == WidgetClass)
			{
				MasterStack->RemoveWidget(*List[i]);
				return;
			}
		}
	}
}

void UToroMasterWidget::PushClassToOverlay(TSubclassOf<UCommonUserWidget> WidgetClass) const
{
	if (MasterOverlay && WidgetClass)
	{
		// Uses const cast here. Dirty but fine as it doesn't actually mutate this and only uses it for outer.
		PushInstanceToOverlay(CreateWidget<UCommonUserWidget>(const_cast<UToroMasterWidget*>(this), WidgetClass));
	}
}

void UToroMasterWidget::PopClassFromOverlay(TSubclassOf<UCommonUserWidget> WidgetClass, const bool bFromEnd) const
{
	if (MasterOverlay && WidgetClass)
	{
		TArray<UWidget*> OverlayWidgets = MasterOverlay->GetAllChildren();
		if (bFromEnd)
		{
			Algo::Reverse(OverlayWidgets);
		}

		for (UWidget* Widget : OverlayWidgets)
		{
			if (Widget->IsA(WidgetClass))
			{
				PopInstanceFromOverlay(Cast<UCommonUserWidget>(Widget));
				return;
			}
		}
	}
}

void UToroMasterWidget::PushInstanceToOverlay(UCommonUserWidget* Widget) const
{
	if (MasterOverlay && Widget)
	{
		MasterOverlay->AddChild(Widget);
	}
}

void UToroMasterWidget::PopInstanceFromOverlay(UCommonUserWidget* Widget) const
{
	if (MasterOverlay && Widget)
	{
		MasterOverlay->RemoveChild(Widget);
	}
}
