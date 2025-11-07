// Copyright (C) RedCraft86. All Rights Reserved.

#include "Nodes/ToroGraphNodeFactory.h"
#include "Nodes/EdToroGraphNode.h"
#include "Nodes/SToroGraphNode.h"
#include "Nodes/EdToroGraphEdge.h"
#include "Nodes/SToroGraphEdge.h"

TSharedPtr<SGraphNode> FToroGraphNodeFactory::CreateNode(UEdGraphNode* Node) const
{
	if (UEdToroGraphNode* EdNode = Cast<UEdToroGraphNode>(Node))
	{
		return SNew(SToroGraphNode, EdNode);
	}
	
	if (UEdToroGraphEdge* EdEdge = Cast<UEdToroGraphEdge>(Node))
	{
		return SNew(SToroGraphEdge, EdEdge);
	}
	
	return FGraphPanelNodeFactory::CreateNode(Node);
}
