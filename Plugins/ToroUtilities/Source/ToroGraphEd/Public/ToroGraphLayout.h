// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "Graph/EdToroGraph.h"
#include "Helpers/ClassGetterMacros.h"
#include "ToroGraphLayout.generated.h"

UCLASS(Config = Editor, DefaultConfig, DisplayName = "ToroGraph Layout")
class UToroGraphLayout final : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UToroGraphLayout(): OptimalDistance(100.f), MaxIteration(50), bFirstPassOnly(false)
	{
		CategoryName = TEXT("Editor");
		SectionName = TEXT("ToroGraphLayout");
	}

	SETTING_GETTER(UToroGraphLayout)

	void Layout(UEdGraph* InGraph);

private:

	UPROPERTY(Transient) TObjectPtr<UEdToroGraph> EdGraph;
	UPROPERTY(Transient) TObjectPtr<UToroDataGraph> Graph;

	UPROPERTY(Config, EditAnywhere, Category = AutoLayout)
		float OptimalDistance;

	UPROPERTY(Config, EditAnywhere, Category = AutoLayout)
		uint8 MaxIteration;

	UPROPERTY(Config, EditAnywhere, Category = AutoLayout)
		bool bFirstPassOnly;

	void InitPass(UToroDataNode* RootNode, const FVector2D& Anchor);
	bool ResolveConflictPass(UToroDataNode* Node);
	bool ResolveConflict(UToroDataNode* LRoot, UToroDataNode* RRoot);
	void GetLeftContour(UToroDataNode* RootNode, int32 Level, TArray<UEdToroGraphNode*>& Contour);
	void GetRightContour(UToroDataNode* RootNode, int32 Level, TArray<UEdToroGraphNode*>& Contour);
	void ShiftSubTree(UToroDataNode* RootNode, const FVector2D& Offset);
	void UpdateParentNodePosition(UToroDataNode* RootNode) const;
};