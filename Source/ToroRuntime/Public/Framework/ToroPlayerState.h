// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerState.generated.h"

UCLASS(NotPlaceable, Blueprintable, BlueprintType, meta = (ChildCanTick = true))
class TORORUNTIME_API AToroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AToroPlayerState();

	template<typename T = AToroPlayerState>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? PC->GetPlayerState<T>() : nullptr;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
};
