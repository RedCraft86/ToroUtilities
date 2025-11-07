// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Engine/DataAsset.h"
#include "ToroDataAsset.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct TOROCORE_API FDatabaseEntry
{
	GENERATED_BODY()

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleDefaultsOnly, Category = Entry, AdvancedDisplay, meta = (EditCondition = false, EditConditionHides))
		FString Label = TEXT("");
#endif

	FDatabaseEntry() {}
	virtual ~FDatabaseEntry() = default;

	virtual bool IsValidData() const { return false; }
};

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TOROCORE_API UToroDataAsset : public UDataAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
public:
	virtual void UpdateData() {}
	virtual FText GetDescription() const { return FText::GetEmpty(); }
protected:
	virtual void PostLoad() override;
	virtual void PostInitProperties() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};

UCLASS(Abstract, NotBlueprintable, BlueprintType)
class TOROCORE_API UToroDatabase : public UToroDataAsset
{
	GENERATED_BODY()

#if WITH_EDITOR
public:
	/* X = Total Data, Y = Valid Data (Swappable, bigger value is always Total) */
	virtual FIntPoint GetDataResults() const { return FIntPoint::ZeroValue; }
	virtual FText GetDescription() const override;
#endif
};