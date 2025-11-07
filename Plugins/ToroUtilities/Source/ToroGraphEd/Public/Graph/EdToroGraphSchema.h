// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "EdGraph/EdGraphSchema.h"
#include "EdToroGraphSchema.generated.h"

USTRUCT()
struct FToroGraphAction_NewNode final : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	FToroGraphAction_NewNode(): NodeTemplate(nullptr) {}
	FToroGraphAction_NewNode(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr)
	{}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
		const FVector2f& Location, bool bSelectNewNode = true) override;

	TObjectPtr<class UEdToroGraphNode> NodeTemplate;
};

USTRUCT()
struct FToroGraphAction_NewEdge final : public FEdGraphSchemaAction
{
	GENERATED_BODY();

	FToroGraphAction_NewEdge(): NodeTemplate(nullptr) {}
	FToroGraphAction_NewEdge(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
		: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping), NodeTemplate(nullptr)
	{}

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
		const FVector2f& Location, bool bSelectNewNode = true) override;

	TObjectPtr<class UEdToroGraphEdge> NodeTemplate;
};

UCLASS()
class UEdToroGraphSchema final : public UEdGraphSchema
{
	GENERATED_BODY()

public:

	UEdToroGraphSchema() {}

	void GetBreakLinkToSubMenuActions(UToolMenu* Menu, UEdGraphPin* InGraphPin);
	virtual EGraphType GetGraphType(const UEdGraph* TestEdGraph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual const FPinConnectionResponse CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const override;
	virtual bool TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual bool CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const override;
	virtual FConnectionDrawingPolicy* CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const override;
	virtual FLinearColor GetPinTypeColor(const FEdGraphPinType& PinType) const override;
	virtual void BreakNodeLinks(UEdGraphNode& TargetNode) const override;
	virtual void BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const override;
	virtual void BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const override;
	virtual UEdGraphPin* DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const override;
	virtual bool SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const override;
	virtual bool IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const override;
	virtual int32 GetCurrentVisualizationCacheID() const override;
	virtual void ForceVisualizationCacheClear() const override;

private:

	static inline int32 CurrentCacheRefreshID = 0;
};
