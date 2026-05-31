// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "GameFramework/HUD.h"
#include "Helpers/WorldGetter.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerHUD.generated.h"

UCLASS(NotPlaceable, Blueprintable, BlueprintType, meta = (ChildCanTick = true))
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

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
	TObjectPtr<USceneComponent> SceneRoot;
};
