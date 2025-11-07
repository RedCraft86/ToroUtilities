// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/ToroContainerWidget.h"
#include "UserInterface/ToroManagedWidget.h"

void UToroContainerWidget::ClearNullEntries()
{
	ManagedWidgets.RemoveAll([](const TWeakObjectPtr<UToroManagedWidget>& Elem)
	{
		return !Elem.IsValid();
	});
}

bool UToroContainerWidget::PushEntry(UToroManagedWidget* Widget)
{
	ClearNullEntries();
	if (Widget && !ManagedWidgets.Contains(Widget))
	{
		ManagedWidgets.Add(Widget);
		return true;
	}
	return false;
}

bool UToroContainerWidget::PopEntry(UToroManagedWidget* Widget)
{
	ClearNullEntries();
	return Widget && ManagedWidgets.Remove(Widget) > 0;
}

void UToroContainerWidget::InitWidget(APlayerController* Controller)
{
	Super::InitWidget(Controller);
	AddToViewport(ZOrder);
}

bool UToroOverlayContainer::PushEntry(UToroManagedWidget* Widget)
{
	if (Super::PushEntry(Widget))
	{
		WidgetStack->AddChild(Widget);
		return true;
	}
	return false;
}

bool UToroOverlayContainer::PopEntry(UToroManagedWidget* Widget)
{
	if (Super::PopEntry(Widget))
	{
		WidgetStack->RemoveChild(Widget);
		return true;
	}
	return false;
}

void UToroOverlayContainer::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!WidgetStack)
	{
		WidgetStack = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass());
		WidgetTree->RootWidget = WidgetStack;
	}
}

bool UToroStackContainer::PushEntry(UToroManagedWidget* Widget)
{
	if (Super::PushEntry(Widget))
	{
		WidgetStack->AddWidgetInstance(*Widget);
		return true;
	}
	return false;
}

bool UToroStackContainer::PopEntry(UToroManagedWidget* Widget)
{
	if (Super::PopEntry(Widget))
	{
		WidgetStack->RemoveWidget(*Widget);
		return true;
	}
	return false;
}

void UToroStackContainer::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (!WidgetStack)
	{
		WidgetStack = WidgetTree->ConstructWidget<UCommonActivatableWidgetStack>(UCommonActivatableWidgetStack::StaticClass());
		WidgetTree->RootWidget = WidgetStack;
	}
}
