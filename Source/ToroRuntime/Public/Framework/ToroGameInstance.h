// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "Helpers/WorldGetter.h"
#include "Engine/GameInstance.h"
#include "ToroGameInstance.generated.h"

/**
 * High-level manager persistent across level transitions. Handles application lifecycle and instance locking.
 */
UCLASS(Blueprintable, BlueprintType)
class TORORUNTIME_API UToroGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class AToroWorldSettings;

public:

	UToroGameInstance() {}

	template<typename T = UToroGameInstance>
	[[nodiscard]] static T* Get(const UObject* ContextObject)
	{
		static_assert(TIsDerivedFrom<T, UToroGameInstance>::Value, 
			"T must derive from UToroGameInstance");

		const UWorld* World = FWorldGetter::Get(ContextObject);
		return IsValid(World) ? World->GetGameInstance<T>() : nullptr;
	}

	/** 
	 * Forcefully restarts the application. 
	 * Releases the instance lock file to ensure the new instance can start without conflict.
	 */
	UFUNCTION(BlueprintCallable, Category = Instance)
		void RestartGame();

	/** Event triggered when a WorldSettings has called its BeginPlay event. */
	UFUNCTION(BlueprintImplementableEvent)
		void WorldBeginPlay();

	/** Event triggered every frame from the world's tick, provided by the active WorldSettings. */
	UFUNCTION(BlueprintImplementableEvent)
		void WorldTick(const float DeltaTime, const bool bGamePaused);

	/** Event triggered when a WorldSettings has called its EndPlay event. */
	UFUNCTION(BlueprintImplementableEvent)
		void WorldEndPlay();

protected:

	FString LockPath;
	FArchive* InstanceLock;

	virtual void ClearInstanceLock();
	virtual void OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldTick(UWorld* InWorld, const float DeltaTime, const bool bGamePaused);
	virtual void OnWorldEndPlay(const TWeakObjectPtr<UWorld> InWorld);

	virtual void Init() override;
	virtual void Shutdown() override;
};
