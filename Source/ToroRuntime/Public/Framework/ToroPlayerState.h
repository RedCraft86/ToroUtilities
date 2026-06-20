// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "ToroPlayerState.generated.h"

/**
 * Container for non-visual player data (names, IDs, persistent stats).
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType, meta = (ChildCanTick = true))
class TORORUNTIME_API AToroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	AToroPlayerState();

	template<typename T = AToroPlayerState>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		static_assert(TIsDerivedFrom<T, AToroPlayerState>::Value, 
			TEXT("T must derive from AToroPlayerState"));

		const UWorld* World = FWorldGetter::Get(ContextObject);
		const APlayerController* PC = IsValid(World) ? World->GetFirstPlayerController() : nullptr;
		return IsValid(PC) ? PC->GetPlayerState<T>() : nullptr;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;
};
