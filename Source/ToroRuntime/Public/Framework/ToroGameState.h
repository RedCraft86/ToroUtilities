// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/WorldGetter.h"
#include "GameFramework/GameStateBase.h"
#include "ToroGameState.generated.h"

UCLASS(NotPlaceable, Blueprintable, BlueprintType, meta = (ChildCanTick = true))
class TORORUNTIME_API AToroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();

	template<typename T = AToroGameState>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetGameState<T>() : nullptr;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
};
