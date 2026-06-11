// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "CommonUserWidget.h"
#include "Components/Overlay.h"
#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "ToroMasterWidget.generated.h"

/**
 * The root UI container. Manages a stack of activatable widgets (Menus) and a global overlay layer.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroMasterWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:

	UToroMasterWidget(const FObjectInitializer& ObjectInit);

	/** 
	 * Instantiates and pushes a widget onto the stack. 
	 * The stack automatically handles input focus and visibility of previous widgets.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PushToStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

	/** 
	 * Removes the specified widget class from the stack. 
	 * If it was the top-most widget, focus returns to the previous item.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PopFromStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

	UCommonActivatableWidgetStack* GetStack() const { return MasterStack; }
	UOverlay* GetOverlay() const { return MasterOverlay; }

protected:

	/** The primary menu stack. Requires a 'CommonActivatableWidgetStack' named 'MasterStack' in the Blueprint. */
	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonActivatableWidgetStack> MasterStack;

	/** Persistent UI layer. Requires an 'Overlay' named 'MasterOverlay' in the Blueprint. */
	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UOverlay> MasterOverlay;
};
