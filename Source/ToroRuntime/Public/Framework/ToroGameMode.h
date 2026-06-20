// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "GameFramework/GameModeBase.h"
#include "ToroGameMode.generated.h"

/**
 * Defines the rules, score, and core class defaults for the current level.
 */
UCLASS(NotPlaceable, Blueprintable, BlueprintType)
class TORORUNTIME_API AToroGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AToroGameMode();

	template<typename T = AToroGameMode>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		static_assert(TIsDerivedFrom<T, AToroGameMode>::Value, 
			TEXT("T must derive from AToroGameMode"));

		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetAuthGameMode<T>() : nullptr;
	}

protected:

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Subobjects)
		TObjectPtr<USceneComponent> SceneRoot;

	/** Cached reference to the global game instance. */
	UPROPERTY(Transient, BlueprintReadOnly, Category = GameMode)
		TWeakObjectPtr<class UToroGameInstance> GameInstance;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
