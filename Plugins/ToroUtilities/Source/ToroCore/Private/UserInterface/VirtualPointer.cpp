// Copyright (C) RedCraft86. All Rights Reserved.

#include "UserInterface/VirtualPointer.h"
#if WITH_EDITOR
#include "Components/CanvasPanelSlot.h"
#endif

UVirtualPointer::UVirtualPointer()
{
	Background.ImageSize = FVector2D(16.0f);
	Background.TintColor = FLinearColor::Gray;
	Background.DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	Background.OutlineSettings.Color = FLinearColor::Black;
	Background.OutlineSettings.bUseBrushTransparency = true;
	Background.OutlineSettings.Width = 2.0f;

	PRAGMA_DISABLE_DEPRECATION_WARNINGS
	Padding = {0.0f};
	BrushColor = {1.0f, 1.0f, 1.0f, 0.5f};
	Visibility = ESlateVisibility::HitTestInvisible;
	PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UVirtualPointer::SimulateClick() const
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	const FVector2D Pos = GetPosition();
	const FPointerEvent MouseEvent(0, SlateApp.CursorPointerIndex, Pos,
		Pos, SlateApp.GetPressedMouseButtons(), EKeys::LeftMouseButton,
		0, SlateApp.GetPlatformApplication()->GetModifierKeys()
	);

	SlateApp.ProcessMouseButtonDownEvent(nullptr, MouseEvent);
	SlateApp.ProcessMouseButtonUpEvent(MouseEvent);
}

#if WITH_EDITOR
void UVirtualPointer::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
	{
		CanvasSlot->SetSize(FVector2D(10.0f));
	}
}
#endif