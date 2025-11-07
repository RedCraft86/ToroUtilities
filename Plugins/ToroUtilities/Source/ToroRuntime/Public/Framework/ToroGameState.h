// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "Helpers/ClassGetterMacros.h"
#include "GameFramework/GameStateBase.h"
#include "ToroGameState.generated.h"

UCLASS(meta = (ChildCanTick = true))
class TORORUNTIME_API AToroGameState final : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();
	
	GLOBAL_CLASS_GETTER(AToroGameState, GetGameState)

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<class UGamePhaseManager> GamePhase;
};
