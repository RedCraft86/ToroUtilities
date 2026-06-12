// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToroWidgetBase.h"
#include "Components/Overlay.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "ToroMasterWidget.generated.h"

/**
 * The root UI container for the game. It acts as the primary viewport layer, 
 * managing a stack of activatable widgets (like Menus and Dialogs) and a persistent 
 * global overlay layer (for Notifications, HUD elements, or Loading screens).
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroMasterWidget : public UToroUserWidget
{
	GENERATED_BODY()

public:

	UToroMasterWidget(const FObjectInitializer& ObjectInit);

	/** 
	 * Instantiates and pushes a widget onto the menu stack. 
	 * The stack automatically handles input focus, back-button navigation, 
	 * and manages the visibility/interactivity of widgets underneath.
	 * 
	 * @param WidgetClass The CommonActivatableWidget class to create and display.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PushToStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

	/** 
	 * Removes the specified widget class from the stack. 
	 * If the removed widget was the top-most item, focus is automatically 
	 * returned to the previous widget in the stack.
	 * 
	 * @param WidgetClass The class type to find and remove from the stack.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
	void PopFromStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

	/** 
	 * Instantiates a widget and adds it to the persistent Overlay layer.
	 * Overlay widgets do not participate in the Activatable Stack's focus logic 
	 * and are typically used for persistent HUD elements.
	 * 
	 * @param WidgetClass The CommonUserWidget class to instantiate.
	 * @return The overlay slot the widget was pushed to.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		UOverlaySlot* PushClassToOverlay(TSubclassOf<UCommonUserWidget> WidgetClass) const;

	/** 
	 * Removes instances of the specified widget class from the Overlay.
	 * 
	 * @param WidgetClass The class type to find and remove.
	 * @param bFromEnd If true, searches and removes the last added instance first.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PopClassFromOverlay(TSubclassOf<UCommonUserWidget> WidgetClass, const bool bFromEnd = true) const;

	/** 
	 * Adds an existing widget instance directly to the Overlay layer.
	 * @param Widget The widget object to add to the overlay container.
	 * @return The overlay slot the widget was pushed to.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		UOverlaySlot* PushInstanceToOverlay(UCommonUserWidget* Widget) const;

	/** 
	 * Removes a specific widget instance from the Overlay layer.
	 * @param Widget The specific instance to remove.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PopInstanceFromOverlay(UCommonUserWidget* Widget) const;

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
