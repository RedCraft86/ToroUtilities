// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "UserWidgets/ToroWidgetBase.h"

UToroUserWidget::UToroUserWidget(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit)
{
}

#if WITH_EDITOR
const FText UToroUserWidget::GetPaletteCategory()
{
	return PaletteCategory;
}
#endif

UToroActivatableWidget::UToroActivatableWidget(const FObjectInitializer& ObjectInit)
	: Super(ObjectInit)
{
}

#if WITH_EDITOR
const FText UToroActivatableWidget::GetPaletteCategory()
{
	return UToroUserWidget::PaletteCategory;
}
#endif
