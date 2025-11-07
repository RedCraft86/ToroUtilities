// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroDataNode.h"
#include "ToroDataGraph.h"

UToroDataNode::UToroDataNode(): NodeID(FGuid::NewGuid()), BackgroundColor(0.1f, 0.1f, 0.1f)
	, ContextMenuName(INVTEXT("Generic Data Node")), ParentLimit(255), ChildLimit(255)
	, CompatibleGraph(UToroDataGraph::StaticClass())
{}

UToroDataEdge* UToroDataNode::GetEdge(UToroDataNode* ChildNode) const
{
	TObjectPtr<UToroDataEdge> const* FoundEdge = Edges.Find(ChildNode);
	return FoundEdge ? *FoundEdge : nullptr;
}

#if WITH_EDITOR
FText UToroDataNode::GetNodeTitle() const
{
	return Name.IsEmptyOrWhitespace() ? ContextMenuName : Name;
}

FText UToroDataNode::GetNodeDescription() const
{
	return Description;
}

void UToroDataNode::SetNodeTitle(const FText& NewTitle)
{
	Name = NewTitle;
}

bool UToroDataNode::CanConnectFrom(UToroDataNode* Other, uint8 NumParentNodes, FString* ErrorMsg)
{
	if (!Other)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Connecting data node is NULL.");
		return false;
	}
	
	if (NumParentNodes >= ParentLimit)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Parent node limit exceeded.");
		return false;
	}

	return true;
}

bool UToroDataNode::CanConnectTo(UToroDataNode* Other, uint8 NumChildNodes, FString* ErrorMsg)
{
	if (!Other)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Target data node is NULL.");
		return false;
	}
	
	if (NumChildNodes >= ChildLimit)
	{
		if (ErrorMsg) *ErrorMsg = TEXT("Child node limit exceeded.");
		return false;
	}

	return true;
}
#endif