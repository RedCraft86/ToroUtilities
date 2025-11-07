// Copyright (C) RedCraft86. All Rights Reserved.

#pragma once

#include "ToroDataNode.h"
#include "EdGraph/EdGraphNode.h"
#include "EdToroGraphNode.generated.h"

UCLASS()
class UEdToroGraphNode final : public UEdGraphNode
{
	GENERATED_BODY()

public:

	UEdToroGraphNode();

	class SToroGraphNode* SNode;

	UPROPERTY(VisibleAnywhere, Instanced, Category = DataGraph)
		TObjectPtr<UToroDataNode> GraphNode;

	void SetNode(UToroDataNode* InNode);
	class UEdToroGraph* GetEdGraph() const;

	FBox2D GetNodeBounds() const;
	FVector2D GetNodeSize() const;

	FText GetDescription() const;
	FText GetIdentifierText() const;
	FLinearColor GetBackgroundColor() const;
	UEdGraphPin* GetInputPin() const { return Pins[0]; }
	UEdGraphPin* GetOutputPin() const { return Pins[1]; }

	virtual void PrepareForCopying() override;
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
};
