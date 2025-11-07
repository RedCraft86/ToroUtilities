// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDataGraph.h"
#include "EdGraph/EdGraph.h"
#include "EdToroGraph.generated.h"

UCLASS()
class UEdToroGraph final : public UEdGraph
{
	GENERATED_BODY()

public:

	UEdToroGraph() {}

	UPROPERTY(Transient) TMap<TObjectPtr<UToroDataNode>, TObjectPtr<class UEdToroGraphNode>> NodeMap;
	UPROPERTY(Transient) TMap<TObjectPtr<UToroDataEdge>, TObjectPtr<class UEdToroGraphEdge>> EdgeMap;

	void RebuildDataGraph();
	UToroDataGraph* GetGraph() const;

	virtual void PostEditUndo() override;
	virtual bool Modify(bool bAlwaysMarkDirty = true) override;
	
private:
	
	void Clear();
	void SortNodes(UToroDataNode* RootNode);
};
