// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroSaveTypes.h"
#include "GameplayTagContainer.h"
#include "ToroGlobalSave.generated.h"

UCLASS()
class TORORUNTIME_API UToroGlobalSave : public UToroSaveObject
{
	GENERATED_BODY()

public:

	UToroGlobalSave();

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGuid> Cutscenes;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGameplayTag> Tutorials;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, uint8> Achievements;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TMap<FGameplayTag, FDateTime> Endings;

	UPROPERTY(BlueprintReadWrite, Category = GlobalSave)
		TSet<FGameplayTag> Content;

	virtual void DeleteData() override;

protected:

	virtual void SerializeData(FArchive& Ar) override;
};
