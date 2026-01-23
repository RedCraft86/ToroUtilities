// Copyright (C) RedCraft86. Licensed under LGPL-3.0 (See LICENSE file for details).

#include "ToroDataGraph.h"
#include "ToroDataEdge.h"
#include "ToroDataNode.h"

UToroDataGraph::UToroDataGraph(): GraphID(FGuid::NewGuid())
#if WITH_EDITOR
	, bCyclical(false)
	, CompatibleNode(UToroDataNode::StaticClass())
	, CompatibleEdge(UToroDataEdge::StaticClass())
#endif
{}

int32 UToroDataGraph::GetNodeDepth() const
{
	if (RootNodes.IsEmpty()) return 0;

	int32 Depth = 0;
	TArray<TObjectPtr<UToroDataNode>> ThisLayer;
	TArray<TObjectPtr<UToroDataNode>> LastLayer = RootNodes;
	while (!LastLayer.IsEmpty())
	{
		for (const TObjectPtr<UToroDataNode>& Node : LastLayer)
		{
			ThisLayer.Append(Node->ChildNodes);
		}

		LastLayer = ThisLayer;
		ThisLayer.Reset();
		Depth++;
	}
	
	return Depth;
}

UToroDataNode* UToroDataGraph::GetNodeByID(const FGuid& InID, const bool bRootFallback) const
{
	if (!InID.IsValid()) return bRootFallback ? RootNodes[0] : nullptr;
	for (const TObjectPtr<UToroDataNode>& Node : AllNodes)
	{
		if (Node->NodeID == InID)
		{
			return Node;
		}
	}
	return bRootFallback ? RootNodes[0] : nullptr;
}

TArray<uint8> UToroDataGraph::ValidateSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
{
	if (RootNodes.IsEmpty()) return {};
	if (InSequence.IsEmpty()) return bRootFallback ? TArray<uint8>{0} : TArray<uint8>();
	
	TArray<uint8> Sequence;
	TObjectPtr<UToroDataNode> LastNode = nullptr;
	TArray<TObjectPtr<UToroDataNode>> LastLayer = RootNodes;
	for (const uint8 Idx : InSequence)
	{
		if (LastLayer.IsValidIndex(Idx))
		{
			LastNode = LastLayer[Idx];
		}
		else break;

		if (LastNode)
		{
			LastLayer = LastNode->ChildNodes;
			Sequence.Add(Idx);
		}
		else break;
	}

	return Sequence.IsEmpty() && bRootFallback ? TArray<uint8>{0} : Sequence;
}

TArray<UToroDataNode*> UToroDataGraph::GetNodesInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
{
	if (RootNodes.IsEmpty()) return {};
	if (InSequence.IsEmpty()) return bRootFallback ? TArray<UToroDataNode*>{RootNodes[0]} : TArray<UToroDataNode*>();
	
	TArray<UToroDataNode*> Nodes;
	TObjectPtr<UToroDataNode> LastNode = nullptr;
	TArray<TObjectPtr<UToroDataNode>> LastLayer = RootNodes;
	for (const uint8 Idx : InSequence)
	{
		if (LastLayer.IsValidIndex(Idx))
		{
			LastNode = LastLayer[Idx];
		}
		else break;

		if (LastNode)
		{
			LastLayer = LastNode->ChildNodes;
			Nodes.Add(LastNode);
		}
		else break;
	}

	return Nodes.IsEmpty() && bRootFallback ? TArray<UToroDataNode*>{ RootNodes[0] } : Nodes;
}

UToroDataNode* UToroDataGraph::GetLeafInSequence(const TArray<uint8>& InSequence, const bool bRootFallback) const
{
	const TArray<UToroDataNode*> Nodes = GetNodesInSequence(InSequence, bRootFallback);
	return Nodes.IsEmpty() ? nullptr : Nodes.Last();
}

TArray<uint8> UToroDataGraph::GetSequenceFromNode(UToroDataNode* InNode) const
{
	if (!InNode || InNode->OwningGraph != this) return {};
	if (InNode->IsRootNode()) return { (uint8)RootNodes.Find(InNode) };
	
	TArray<uint8> Sequence;
	TObjectPtr<UToroDataNode> LastParent = InNode->ParentNodes[0];
	while (LastParent)
	{
		Sequence.Insert(LastParent->ChildNodes.Find(InNode), 0);
		LastParent = LastParent->IsRootNode() ? nullptr : LastParent->ParentNodes[0];
	}

	return Sequence;
}

TArray<uint8> UToroDataGraph::GetSequenceFromNodeID(const FGuid& InID, const TArray<uint8>& InSequence, const bool bRootFallback) const
{
	UToroDataNode* TargetNode = GetNodeByID(InID, bRootFallback);
	if (!TargetNode)
	{
		return {0};
	}

	if (InSequence.IsEmpty())
	{
		return GetSequenceFromNode(TargetNode);
	}
	
	bool bWalked = false;
	TArray<uint8> Sequence = WalkSequence(InSequence, [TargetNode, &bWalked](const UToroDataNode* CurrentNode)
	{
		if (CurrentNode == TargetNode)
		{
			bWalked = true;
			return true;
		}
		return false;
	}, true);

	return bWalked ? Sequence : GetSequenceFromNode(TargetNode);
}

TArray<uint8> UToroDataGraph::WalkSequence(const TArray<uint8>& InSequence, const TFunction<bool(const UToroDataNode*)>& StopWhen, const bool bInclusive) const
{
	if (!StopWhen || RootNodes.IsEmpty() || InSequence.IsEmpty())
	{
		return {};
	}

	TArray<uint8> Sequence;
	TArray<TObjectPtr<UToroDataNode>> ThisLayer = RootNodes;
	for (const int32 Idx : InSequence)
	{
		if (!ThisLayer.IsValidIndex(Idx))
		{
			break;
		}
		
		TObjectPtr<UToroDataNode> Node = ThisLayer[Idx];
		if (!Node)
		{
			break;
		}

		if (StopWhen(Node))
		{
			if (bInclusive)
			{
				Sequence.Add(Idx);
			}
			break;
		}

		Sequence.Add(Idx);
		ThisLayer = Node->ChildNodes;
	}
	
	return Sequence;
}

#if WITH_EDITOR
void UToroDataGraph::ClearGraph()
{
	for (UToroDataNode* Node : AllNodes)
	{
		if (!Node) continue;
		Node->ParentNodes.Empty();
		Node->ChildNodes.Empty();
		Node->Edges.Empty();
	}
	AllNodes.Empty();
	RootNodes.Empty();
}

FText UToroDataGraph::GetDescription() const
{
	return FText::Format(INVTEXT("{0} Node(s) | {1} Root(s){2}"), AllNodes.Num(),
		RootNodes.Num(), bCyclical ? INVTEXT(" | Cyclical") : FText::GetEmpty());
}
#endif