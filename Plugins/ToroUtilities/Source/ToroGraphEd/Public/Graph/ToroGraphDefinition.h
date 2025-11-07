// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroGraphEd.h"
#include "ToroDataGraph.h"
#include "ToroGraphAssetEditor.h"
#include "AssetFactories/ToroAssetDefinition.h"
#include "ToroGraphDefinition.generated.h"

UCLASS()
class UToroGraphDefinition final : public UToroAssetDefinition
{
	GENERATED_BODY()

public:

	UToroGraphDefinition() {}

	virtual FLinearColor GetAssetColor() const override { return FColor(0, 120, 255); }
	virtual TSoftClassPtr<UObject> GetAssetClass() const override { return UToroDataGraph::StaticClass(); }
	virtual FText GetAssetDisplayName() const override
	{
		return NSLOCTEXT("ToroGraphEd", "DataGraphAsset", "Data Graph");
	}

	virtual const FSlateBrush* GetIconBrush(const FAssetData& InAssetData, const FName InClassName) const override
	{
		return FToroGraphEdStyle::Get().GetBrush("ToroGraph.AutoArrange");
	}
	
	virtual EAssetCommandResult OpenAssets(const FAssetOpenArgs& OpenArgs) const override
	{
		const EToolkitMode::Type Mode = OpenArgs.ToolkitHost.IsValid() ?
			EToolkitMode::WorldCentric : EToolkitMode::Standalone;

		for (UObject* Object : OpenArgs.LoadObjects<UObject>())
		{
			if (UToroDataGraph* Graph = Cast<UToroDataGraph>(Object))
			{
				const TSharedRef<FToroGraphAssetEditor> GraphEditor(new FToroGraphAssetEditor());
				GraphEditor->InitEditor(Mode, OpenArgs.ToolkitHost, Graph);
			}
		}

		return EAssetCommandResult::Handled;
	}
};
