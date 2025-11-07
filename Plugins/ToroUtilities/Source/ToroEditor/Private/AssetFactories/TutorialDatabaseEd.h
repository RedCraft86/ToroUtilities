// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroEditor.h"
#include "AssetFactories/ToroAssetFactory.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "Gameplay/Tutorials/TutorialDatabase.h"
#include "TutorialDatabaseEd.generated.h"

UCLASS()
class TOROEDITOR_API UTutorialDatabaseFactory final : public UToroAssetFactory
{
	GENERATED_BODY()

public:

	UTutorialDatabaseFactory()
	{
		AssetName = TEXT("TutorialDatabase");
		SupportedClass = UTutorialDatabase::StaticClass();
	}
};

UCLASS()
class TOROEDITOR_API UTutorialDatabaseDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	virtual FLinearColor GetAssetColor() const override { return FColor(20, 150, 120); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UTutorialDatabase::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroEditor", "TutorialDatabaseName", "Tutorial Database");
	}

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		return FToroEditorStyle::Get().GetBrush("ClassThumbnail.TutorialDatabase");
	}
};