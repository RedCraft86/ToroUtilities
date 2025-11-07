// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagMacros.h"
#include "MiscObjects/ToroDataAsset.h"
#include "TutorialDatabase.generated.h"

namespace TutorialTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Tutorial)
	TORORUNTIME_API CREATE_TAG_VERIFIER(Tutorial)
}

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FTutorialEntry : public FDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Entry)
		TSoftObjectPtr<UTexture2D> Image;

	UPROPERTY(EditAnywhere, Category = Entry)
		FText Name;

	UPROPERTY(EditAnywhere, Category = Entry)
		FText Description;

	virtual bool IsValidData() const override
	{
		return !Name.IsEmptyOrWhitespace()
			&& !Description.IsEmptyOrWhitespace();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UTutorialDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UTutorialDatabase() {}

	UPROPERTY(EditAnywhere, Category = Database, meta = (ForceInlineRow, Categories = "Tutorial", TitleProperty = "Label"))
		TMap<FGameplayTag, FTutorialEntry> Entries;

	const FTutorialEntry* GetEntry(const FGameplayTag& Key) const;

#if WITH_EDITOR
	virtual FIntPoint GetDataResults() const override;
protected:
	virtual void UpdateData() override;
#endif
};
