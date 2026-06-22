// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0). 
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "ToroSaveGame.h"
#include "Helpers/WorldGetter.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ToroSaveManager.generated.h"

/**
 * A subsystem used to manage the lifecycle of save game objects for the entire duration of the game instance.
 * Serves as a cache for save objects, ensuring that only one instance of a specific class exists at a time.
 */
UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroSaveManager final : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UToroSaveManager() {}

	[[nodiscard]] static UToroSaveManager* Get(const UObject* ContextObject)
	{
		const UWorld* World = FWorldGetter::Get(ContextObject);
		const UGameInstance* GI = IsValid(World) ? World->GetGameInstance() : nullptr;
		return IsValid(GI) ? GI->GetSubsystem<UToroSaveManager>() : nullptr;
	}

	/**
	 * Retrieves an existing save object from the cache or creates a new instance if none exists.
	 * @param Class The specific UToroSaveGame class to find or instantiate.
	 * @return The cached or newly created save object instance or null if Class is null.
	 */
	UFUNCTION(BlueprintCallable, Category = SaveSystem, meta = (DetermineOutputType = Class))
		UToroSaveGame* GetOrCreateSaveObject(UPARAM(meta=(AllowAbstract=false)) const TSubclassOf<UToroSaveGame> Class);

	/**
	 * Get an existing save object instance or create a new instance if none exist.
	 * @tparam T The class type, which must derive from UToroSaveGame.
	 * @return The instance of T, or nullptr if the system fails to create/find the object.
	 */
	template<typename T>
	T* GetOrCreateSaveObject()
	{
		static_assert(TIsDerivedFrom<T, UToroSaveGame>::Value, 
			TEXT("T must derive from UToroSaveGame"));

		return Cast<T>(GetOrCreateSaveObject(T::StaticClass()));
	}

private:

	UPROPERTY(Transient)
		TMap<TSubclassOf<UToroSaveGame>, TObjectPtr<UToroSaveGame>> SaveObjects;

	virtual void Deinitialize() override;
};
