// Copyright (C) 2026 RedCraft86. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#include "UserWidgets/ToroMasterWidget.h"
#include "Components/OverlaySlot.h"

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

UOverlaySlot* UToroMasterWidget::PushClassToOverlay(TSubclassOf<UCommonUserWidget> WidgetClass) const
{
	if (MasterOverlay && WidgetClass)
	{
		// Uses const cast here. Dirty but fine as it doesn't actually mutate this and only uses it for outer.
		return PushInstanceToOverlay(CreateWidget<UCommonUserWidget>(const_cast<UToroMasterWidget*>(this), WidgetClass));
	}

	return nullptr;
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

UOverlaySlot* UToroMasterWidget::PushInstanceToOverlay(UCommonUserWidget* Widget) const
{
	if (MasterOverlay && Widget)
	{
		return Cast<UOverlaySlot>(MasterOverlay->AddChild(Widget));
	}

	return nullptr;
}

void UToroMasterWidget::PopInstanceFromOverlay(UCommonUserWidget* Widget) const
{
	if (MasterOverlay && Widget)
	{
		MasterOverlay->RemoveChild(Widget);
	}
}
