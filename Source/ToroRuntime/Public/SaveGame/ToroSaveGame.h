// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0).
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UE5Coro.h"
#include "GameFramework/SaveGame.h"
#include "ToroSaveGame.generated.h"

/** High-level state of the save/load process. */
UENUM(BlueprintType)
enum class EToroSaveOperation : uint8
{
	None,
	Saving,
	Loading
};

/** Detailed result status for a completed save/load operation. */
UENUM(BlueprintType)
enum class EToroSaveLoadStatus : uint8
{
	/** Currently in saving or loading process. */
	Waiting,

	/** Operation completed successfully. */
	Succeeded,

	/** Oodle Error: Failed to compress data. */
	CompressFailed,

	/** Oodle Error: Failed to decompress data. */
	DecompressFailed,

	/** IO Error: Failed to write the file to disk. */
	FileWriteFailed,

	/** IO Error: Could not find or read the file from disk. */
	FileReadFailed,

	/** Something went wrong and did not get properly serialized in SerializeData(). */
	SerializeFailed
};

/**
 * The base class for custom SaveGame objects in the Toro framework.
 * Supports asynchronous file I/O using UE5Coro coroutines.
 * @note Save data is compressed before writing to disk and vice versa with Oodle.
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UToroSaveGame()
		: SaveName(NAME_None) 
	{}

	const FName& GetSaveName() const { return SaveName; }
	EToroSaveOperation GetCurrentOperation() const { return Operation; }

	/** 
	 * Asynchronously serializes data and writes it to the specified slot file. 
	 * @return A coroutine that resolves to the final success/failure status.
	 */
	virtual UE5Coro::TCoroutine<EToroSaveLoadStatus> SaveToFile(const uint8 Slot);

	/** 
	 * Asynchronously reads a file from disk and populates this object's data.
	 * @return A coroutine that resolves to the final success/failure status.
	 */
	virtual UE5Coro::TCoroutine<EToroSaveLoadStatus> LoadFromFile(const uint8 Slot);

protected:

	/** The internal identifier for this save (e.g., "GameData", "PersistentData"). */
	UPROPERTY(BlueprintReadOnly, Category = SaveObject)
		FName SaveName;

	/** The current runtime state of this save object. */
	UPROPERTY(BlueprintReadOnly, Category = SaveObject)
		EToroSaveOperation Operation;

	virtual FString GetSavePath(const uint8 Slot) const;
	virtual void SerializeData(FArchive& Ar) PURE_VIRTUAL(UToroSaveGame::SerializeData,)
};
