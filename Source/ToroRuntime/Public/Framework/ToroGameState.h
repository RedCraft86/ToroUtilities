// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "GameFramework/GameStateBase.h"
#include "ToroGameState.generated.h"

/**
 * Synchronized state of the game (scores, timers, objective progress).
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType, meta = (ChildCanTick = true))
class TORORUNTIME_API AToroGameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	AToroGameState();

	template<typename T = AToroGameState>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		static_assert(TIsDerivedFrom<T, AToroGameState>::Value, 
			TEXT("T must derive from AToroGameState"));

		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetGameState<T>() : nullptr;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
};
