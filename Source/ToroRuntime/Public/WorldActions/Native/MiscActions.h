// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "AsyncMessageId.h"
#include "LevelSequenceActor.h"
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

/** 
 * Triggers a Level Sequence (Cinematic) within the world. 
 * Allows adjusting how the sequence is played out.
 */
UCLASS(NotBlueprintable, DisplayName = "[Misc] Level Sequence")
class TORORUNTIME_API UWorldAction_LevelSequence final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_LevelSequence() {}

private:

	/** 
	 * The specific LevelSequenceActor in the world that should begin playing. 
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		TSoftObjectPtr<ALevelSequenceActor> Sequence;

	/** 
	 * The sequence will go reverse from its current playback position. 
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		bool bReverse;

	/** 
	 * The sequence will rewind back to the start (to the end if reversing) before playing. 
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		bool bRestart;

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
 * Broadcasts a message via the AsyncMessageSystem to all bound listeners.
 * Used for triggering events like UI updates, achievement unlocks, or world state changes.
 */
UCLASS(NotBlueprintable, DisplayName = "[Misc] Async Message")
class TORORUNTIME_API UWorldAction_AsyncMessage final : public UWorldActionBase
{
	GENERATED_BODY()

public:

	UWorldAction_AsyncMessage() {}

private:

	/** 
	 * The unique identifier/channel for this message. 
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		FAsyncMessageId MessageId;

	/** 
	 * Flexible data payload. 
	 * In the Editor, you can select any USTRUCT type to send with this message.
	 */
	UPROPERTY(EditAnywhere, Category = Action)
		FInstancedStruct Payload;

	virtual void OnExecute_Implementation() override;
};