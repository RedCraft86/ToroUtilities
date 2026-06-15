// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "LevelSequenceActor.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "WorldActions/WorldActionBase.h"
#include "MiscActions.generated.h"

/** 
 * Handles asynchronous level streaming. 
 * Allows loading multiple levels into memory and unloading others in a single batch.
 */
UCLASS(NotBlueprintable, DisplayName = "[Misc] Level Streaming")
class TORORUNTIME_API UWorldAction_LevelStreaming final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_LevelStreaming() {}

private:

	/** 
	 * Map of levels to load. 
	 * Key: The World asset. 
	 * Value: True if the level should be made visible immediately upon loading completion.
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		TMap<TSoftObjectPtr<UWorld>, bool> Load;

	/** A collection of levels that should be removed from memory. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSet<TSoftObjectPtr<UWorld>> Unload;

	virtual void OnExecute_Implementation() override;
};

/** Triggers a Level Sequence (Cinematic) within the world. */
UCLASS(NotBlueprintable, DisplayName = "[Misc] Level Sequence")
class TORORUNTIME_API UWorldAction_LevelSequence final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_LevelSequence() {}

private:

	/** The specific LevelSequenceActor in the world that should begin playing. */
	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<ALevelSequenceActor> Sequence;

	virtual void OnExecute_Implementation() override;
};

/** 
 * Calls a 'Remote Event' in the Level Blueprint. 
 * This is a way to trigger logic defined strictly within the current map's level blueprint.
 */
UCLASS(NotBlueprintable, DisplayName = "[Misc] Remote Event")
class TORORUNTIME_API UWorldAction_RemoteEvent final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_RemoteEvent()
		: EventName(NAME_None) 
	{}

private:

	/** The case-sensitive name of the Custom Event to trigger in the Level Blueprint. */
	UPROPERTY(EditAnywhere, Category = Action)
		FName EventName;

	virtual void OnExecute_Implementation() override;
};

/**
 * A concrete world action that updates the global metadata store in the ToroRuntimeSubsystem.
 * This is useful for persisting high-level game state (e.g., "QuestStarted", "DifficultyLevel") 
 * that needs to be accessed by various systems across different levels.
 */
UCLASS(NotBlueprintable, DisplayName = "[Misc] Global Metadata")
class TORORUNTIME_API UWorldAction_GlobalMetadata final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_GlobalMetadata() {}

private:

	/** The metadata key/tag to add/update (filtered to the 'Flag' category). */
	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "Flag"))
		FGameplayTag Key;

	/** 
	 * The polymorphic data to store. 
	 * You can use any USTRUCT marked as 'BlueprintType' to pass complex data into the metadata store.
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		FInstancedStruct Value;

	virtual void OnExecute_Implementation() override;
};

/** 
 * Invokes a Global Event on the ToroRuntimeSubsystem bus.
 * Decouples logic by sending a tagged payload that other systems can listen for.
 */
UCLASS(NotBlueprintable, DisplayName = "[Misc] Global Event")
class TORORUNTIME_API UWorldAction_GlobalEvent final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_GlobalEvent()
		: Instigator(nullptr) 
	{}

private:

	/** The event channel/tag to invoke (filtered to the 'Event' category). */
	UPROPERTY(EditAnywhere, Category = Action, meta = (Categories = "Event"))
		FGameplayTag Key;

	/** Optional actor that triggered the event. If unset, the action uses the current WorldContext. */
	UPROPERTY(EditAnywhere, Category = Action, DisplayName = "Instigator (Optional)")
		TSoftObjectPtr<AActor> Instigator;

	/** Arbitrary data structure passed to all listeners of this event. */
	UPROPERTY(EditAnywhere, Category = Action)
		FInstancedStruct Payload;

	virtual void OnExecute_Implementation() override;
};