// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "Gameplay/Achievements/AchievementDatabase.h"
#include "AchieveDatabaseEd.generated.h"

UCLASS()
class TOROEDITOR_API UAchieveDatabaseFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UAchieveDatabaseFactory()
	{
		AssetName = TEXT("AchievementDatabase");
		SupportedClass = UAchievementDatabase::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UAchieveDatabaseDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(255, 193, 37); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UAchievementDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "AchievementDatabaseName", "Achievement Database");
	}

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		return FToroEditorStyle::Get().GetBrush("ClassThumbnail.AchievementDatabase");
	}
};