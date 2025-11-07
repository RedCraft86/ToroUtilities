// Copyright (C) RedCraft86. All Rights Reserved.

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

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TArray<uint8> Sequence;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		FInventoryArchiveSave Archives;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		TMap<FGameplayTag, FInventoryItemSave> Items;

	UPROPERTY(BlueprintReadOnly, Category = GameSave)
		FSoftObjectPath Equipment;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};