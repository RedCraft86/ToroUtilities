// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDataEdge.h"
#include "EdGraph/EdGraphNode.h"
#include "EdToroGraphEdge.generated.h"

class UEdToroGraphNode;

UCLASS()
class UEdToroGraphEdge final : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UEdToroGraphEdge();

	class SToroGraphEdge* SEdge;

	UPROPERTY(VisibleAnywhere, Instanced, Category = DataGraph)
		TObjectPtr<UToroDataEdge> GraphEdge;

	void SetEdge(UToroDataEdge* Edge);
	class UEdToroGraph* GetEdGraph() const;
	void CreateConnections(const UEdToroGraphNode* Start, const UEdToroGraphNode* End);

	UEdToroGraphNode* GetEndNode();
	UEdToroGraphNode* GetStartNode();
	UEdGraphPin* GetInputPin() const { return Pins[0]; }
	UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	virtual void PrepareForCopying() override;
	virtual void AllocateDefaultPins() override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
