// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Helpers/GameplayTagMacros.h"
#include "MiscObjects/ToroDataAsset.h"
#include "AchievementDatabase.generated.h"

namespace AchievementTags
{
	TORORUNTIME_API DECLARE_GAMEPLAY_TAG(Achievement)
	TORORUNTIME_API CREATE_TAG_VERIFIER(Achievement)
}

USTRUCT(BlueprintInternalUseOnly)
struct TORORUNTIME_API FAchievementEntry : public FDatabaseEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Entry)
		TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category = Entry)
		FText Name;

	UPROPERTY(EditAnywhere, Category = Entry)
		FText Description;

	UPROPERTY(EditAnywhere, Category = Entry, meta = (ClampMin = 1, UIMin = 1, ClampMax = 255, UIMax = 255))
		uint8 Requirement;

	FAchievementEntry(): Requirement(1) {}

	virtual bool IsValidData() const override
	{
		return !Icon.IsNull() && !Name.IsEmptyOrWhitespace();
	}
};

UCLASS(NotBlueprintable, BlueprintType)
class TORORUNTIME_API UAchievementDatabase final : public UToroDatabase
{
	GENERATED_BODY()

public:

	UAchievementDatabase() {}

	UPROPERTY(EditAnywhere, Category = Database, meta = (ForceInlineRow, Categories = "Achievement", TitleProperty = "Label"))
		TMap<FGameplayTag, FAchievementEntry> Entries;

	const FAchievementEntry* GetEntry(const FGameplayTag& Key) const;

#if WITH_EDITOR
	virtual FIntPoint GetDataResults() const override;
protected:
	virtual void UpdateData() override;
#endif
};
