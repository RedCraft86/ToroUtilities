// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#pragma once

#include "ToroSaveTypes.h"
#include "GameplayTagContainer.h"
#include "Inventory/InventorySave.h"
#include "ToroGameSave.generated.h"

UCLASS()
class TORORUNTIME_API UToroGameSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGameSave();

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		float PlayTime;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGuid, bool> Progress;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TArray<uint8> Sequence;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		FInventoryArchiveSave Archives;

	// UPROPERTY(BlueprintReadOnly, Category = GameSave)
	// 	FInventoryItemSave Items;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		FSoftObjectPath Equipment;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};