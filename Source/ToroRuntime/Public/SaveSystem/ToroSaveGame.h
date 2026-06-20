// Copyright (C) 2026 Tayzar Linn. Licensed under the GNU Lesser General Public License v3.0 (LGPL-3.0). 
// See the LICENSE file in the project root, or <https://www.gnu.org/licenses/lgpl-3.0.html>.

#pragma once

#include "UE5Coro.h"
#include "GameFramework/SaveGame.h"
#include "ToroSaveGame.generated.h"

/** High-level state of the current save/load process. */
UENUM(BlueprintType)
enum class EToroSaveOperation : uint8
{
	None,      /* No active operation. */
	Saving,    /* Data is being serialized or written to disk. */
	Loading    /* Data is being read from disk or deserialized. */
};

/** Detailed result status for a completed save/load operation. */
UENUM(BlueprintType)
enum class EToroSaveLoadStatus : uint8
{
	Waiting,           /* Operation is currently in progress. */
	Succeeded,         /* Operation completed successfully. */
	CompressFailed,    /* Oodle Error: Failed to compress data during save. */
	DecompressFailed,  /* Oodle Error: Failed to decompress data during load. */
	FileWriteFailed,   /* IO Error: Failed to write the .sar file to disk. */
	FileReadFailed,    /* IO Error: Could not find or access the .sar file on disk. */
	SerializeFailed    /* Archive Error: Memory corruption or structural mismatch during serialization. */
};

/**
 * Abstract base class for the ToroUtilities Save System.
 * Subclasses should implement SerializeData to define what gets saved.
 * Uses Oodle Compression and asynchronous file IO for large data.
 */
UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroSaveGame : public UObject
{
	GENERATED_BODY()

	/** Name of This Object if bSerializeSelf is enabled. */
	static inline const FArchiveFieldName SelfFieldName = TEXT("Metadata");

public:

	UToroSaveGame()
		: SaveName(NAME_None) 
	{}

	const FName& GetSaveName() const { return SaveName; }
	EToroSaveOperation GetCurrentOperation() const { return CurrentOperation; }
	virtual FString GetSavePath(const uint8 Slot) const;

	/** 
	 * Serializes data, then asynchronously compresses it with Oodle and writes to disk.
	 * @note Uses TStrongObjectPtr internally to prevent GC during the async task.
	 * @param Slot If >0, save path will be "/SaveGames/Slot_XX/SaveName.sar"
	 * @param OutStatus The result status of the save operation, whether it succeeded or errored
	 */
	UFUNCTION(BlueprintCallable, Category = SaveSystem, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine SaveObject(FLatentActionInfo LatentInfo, EToroSaveLoadStatus& OutStatus, const uint8 Slot);
	virtual UE5Coro::TCoroutine<EToroSaveLoadStatus> SaveToFile(const uint8 Slot);

	/** 
	 * Asynchronously reads a file from disk and decompresses it, then serializes the data.
	 * @note Uses TStrongObjectPtr internally to prevent GC during the async task.
	 * @param Slot If >0, save path will be "/SaveGames/Slot_XX/SaveName.sar"
	 * @param OutStatus The result status of the save operation, whether it succeeded or errored
	 */
	UFUNCTION(BlueprintCallable, Category = SaveSystem, meta = (Latent, LatentInfo = LatentInfo))
		FVoidCoroutine LoadObject(FLatentActionInfo LatentInfo, EToroSaveLoadStatus& OutStatus, const uint8 Slot);
	virtual UE5Coro::TCoroutine<EToroSaveLoadStatus> LoadFromFile(const uint8 Slot);

	virtual UWorld* GetWorld() const override;

protected:

	/** The name of the save file (e.g., "GameData", "PersistentData"). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SaveObject)
		FName SaveName;

	/** Serialize the save object itself. Useful if there's any metadata properties. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SaveObject)
		bool bSerializeSelf;

	/** The current runtime state. Updated automatically during Save/Load. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveObject)
		EToroSaveOperation CurrentOperation;

	/** 
	 * Core extension point for serialization in subclasses. Pure Virtual, must override.
	 * @param Record The structured archive root record.
	 */
	virtual void SerializeData(FStructuredArchive::FRecord& Record) PURE_VIRTUAL(UToroSaveGame::SerializeData,);
};
