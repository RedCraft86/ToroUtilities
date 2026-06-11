// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "GameFramework/HUD.h"
#include "Helpers/WorldGetter.h"
#include "GameFramework/PlayerController.h"
#include "UserWidgets/ToroMasterWidget.h"
#include "ToroPlayerHUD.generated.h"

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
		UToroMasterWidget* GetMasterWidget() { return MasterWidget; }

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(Transient)
		TObjectPtr<UToroMasterWidget> MasterWidget;

	virtual void BeginPlay() override;
};
