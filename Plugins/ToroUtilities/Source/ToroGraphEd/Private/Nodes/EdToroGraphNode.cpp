// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/EdToroGraphNode.h"
#include "Nodes/SToroGraphNode.h"
#include "Graph/EdToroGraph.h"

UEdToroGraphNode::UEdToroGraphNode(): SNode(nullptr)
{
	GraphNode = nullptr;
	bCanRenameNode = true;
}

void UEdToroGraphNode::SetNode(UToroDataNode* InNode)
{
	GraphNode = InNode;
}

UEdToroGraph* UEdToroGraphNode::GetEdGraph() const
{
	return Cast<UEdToroGraph>(GetGraph());
}

FBox2D UEdToroGraphNode::GetNodeBounds() const
{
	const FVector2D Pos(NodePosX, NodePosY);
	return FBox2D(Pos, Pos + GetNodeSize());
}

FVector2D UEdToroGraphNode::GetNodeSize() const
{
	return SNode->GetCachedGeometry().GetLocalSize();
}

FText UEdToroGraphNode::GetDescription() const
{
	return GraphNode ? GraphNode->GetNodeDescription() : INVTEXT("Unknown Node");
}

FText UEdToroGraphNode::GetIdentifierText() const
{
	return GraphNode ? FText::FromString(GraphNode->NodeID.ToString()) : INVTEXT("Unknown ID");
}

FLinearColor UEdToroGraphNode::GetBackgroundColor() const
{
	return GraphNode ? GraphNode->BackgroundColor : FLinearColor::Black;
}

void UEdToroGraphNode::PrepareForCopying()
{
	GraphNode->Rename(nullptr, this, REN_DontCreateRedirectors | REN_DoNotDirty);
}

void UEdToroGraphNode::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, TEXT("MultipleNodes"), FName(), TEXT("In"));
	CreatePin(EGPD_Output, TEXT("MultipleNodes"), FName(), TEXT("Out"));
}

void UEdToroGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);
	if (FromPin && GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
	{
		FromPin->GetOwningNode()->NodeConnectionListChanged();
	}
}

FText UEdToroGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return GraphNode ? GraphNode->GetNodeTitle() : Super::GetNodeTitle(TitleType);
}
