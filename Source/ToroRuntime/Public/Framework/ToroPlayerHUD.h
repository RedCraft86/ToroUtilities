// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "CommonUserWidget.h"
#include "CommonActivatableWidget.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Components/Overlay.h"
#include "GameFramework/HUD.h"
#include "Helpers/WorldGetter.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerHUD.generated.h"

/**
 * The root UI container. Manages a stack of activatable widgets (Menus) and a global overlay layer.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroMasterWidget : public UCommonUserWidget
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

/**
 * Manages the lifetime and visibility of the UToroMasterWidget.
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType, PrioritizeCategories = (Settings), meta = (ChildCanTick = true))
class TORORUNTIME_API AToroPlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	AToroPlayerHUD();

	template<typename T = AToroPlayerHUD>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? PC->GetHUD<T>() : nullptr;
	}

	/** Returns the master UI widget container. */
	UFUNCTION(BlueprintPure, Category = HUD)
		UToroMasterWidget* GetMainGameHUD() { return MasterWidget; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	/** The instantiated Master Widget. Created during BeginPlay using the class from ToroRuntimeSettings. */
	UPROPERTY(Transient)
		TObjectPtr<UToroMasterWidget> MasterWidget;

	virtual void BeginPlay() override;
};
