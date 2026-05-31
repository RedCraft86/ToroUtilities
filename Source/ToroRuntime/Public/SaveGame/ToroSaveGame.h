// Copyright (C) RedCraft86 2026. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "UE5Coro.h"
#include "GameFramework/SaveGame.h"
#include "ToroSaveGame.generated.h"

UENUM(BlueprintType)
enum class EToroSaveOperation : uint8
{
	None,
	Saving,
	Loading
};

UENUM(BlueprintType)
enum class EToroSaveLoadStatus : uint8
{
	Waiting,
	Succeeded,
	CompressFailed,
	DecompressFailed,
	FileWriteFailed,
	FileReadFailed,
	SerializeFailed,
};

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TORORUNTIME_API UToroSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UToroSaveGame()
		: SaveName(NAME_None) 
	{}

	EToroSaveOperation GetCurrentOperation() const { return Operation; }
	virtual UE5Coro::TCoroutine<EToroSaveLoadStatus> SaveToFile(const uint8 Slot);
	virtual UE5Coro::TCoroutine<EToroSaveLoadStatus> LoadFromFile(const uint8 Slot);

protected:

	FName SaveName;
	EToroSaveOperation Operation;

	virtual FString GetSavePath(const uint8 Slot) const;
	virtual void SerializeData(FArchive& Ar) PURE_VIRTUAL(UToroSaveGame::SerializeData,)
};
