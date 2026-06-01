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

UCLASS(Abstract, Blueprintable, BlueprintType)
class TORORUNTIME_API UToroMasterWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:

	UToroMasterWidget(const FObjectInitializer& ObjectInit);

	/** Pushes a widget into the end of the MasterStack. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PushToStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

	/** Pops off a widget with a matching class from the end of the MasterStack. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = HUD)
		void PopFromStack(TSubclassOf<UCommonActivatableWidget> WidgetClass) const;

	UCommonActivatableWidgetStack* GetStack() const { return MasterStack; }
	UOverlay* GetOverlay() const { return MasterOverlay; }

protected:

	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UCommonActivatableWidgetStack> MasterStack;

	UPROPERTY(BlueprintReadOnly, Category = Subobjects, meta = (BindWidget))
		TObjectPtr<UOverlay> MasterOverlay;
};

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

	UFUNCTION(BlueprintPure, Category = HUD)
		UToroMasterWidget* GetMainGameHUD() { return MasterWidget; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(Transient)
		TObjectPtr<UToroMasterWidget> MasterWidget;

	virtual void BeginPlay() override;
};
