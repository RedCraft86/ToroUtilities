// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/EdToroGraph.h"
#include "Nodes/EdToroGraphNode.h"
#include "Nodes/EdToroGraphEdge.h"
#include "ToroGraphEd.h"

void UEdToroGraph::RebuildDataGraph()
{
	UToroDataGraph* Graph = GetGraph();

	Clear();
	for (int i = 0; i < Nodes.Num(); ++i)
	{
		if (UEdToroGraphNode* EdNode = Cast<UEdToroGraphNode>(Nodes[i]))
		{
			if (EdNode->GraphNode == nullptr) continue;
			UToroDataNode* DataGraphNode = EdNode->GraphNode;
			NodeMap.Add(DataGraphNode, EdNode);
			Graph->AllNodes.Add(DataGraphNode);
			
			for (int PinIdx = 0; PinIdx < EdNode->Pins.Num(); ++PinIdx)
			{
				UEdGraphPin* Pin = EdNode->Pins[PinIdx];
				if (Pin->Direction != EGPD_Output) continue;
				for (int LinkToIdx = 0; LinkToIdx < Pin->LinkedTo.Num(); ++LinkToIdx)
				{
					UToroDataNode* ChildNode = nullptr;
					if (const UEdToroGraphNode* EdNode_Child = Cast<UEdToroGraphNode>
						(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						ChildNode = EdNode_Child->GraphNode;
					}
					else if (UEdToroGraphEdge* EdNode_Edge = Cast<UEdToroGraphEdge>
						(Pin->LinkedTo[LinkToIdx]->GetOwningNode()))
					{
						if (const UEdToroGraphNode* Child = EdNode_Edge->GetEndNode())
						{
							ChildNode = Child->GraphNode;
						}
					}

					if (ChildNode)
					{
						DataGraphNode->ChildNodes.Add(ChildNode);
						ChildNode->ParentNodes.Add(DataGraphNode);
					}
					else
					{
						UE_LOG(LogToroGraphEd, Error, TEXT("UEdToroGraph::RebuildDataGraph can't find child node"));
					}
				}
			}
		}
		else if (UEdToroGraphEdge* EdgeNode = Cast<UEdToroGraphEdge>(Nodes[i]))
		{
			const UEdToroGraphNode* StartNode = EdgeNode->GetStartNode();
			const UEdToroGraphNode* EndNode = EdgeNode->GetEndNode();
			UToroDataEdge* Edge = EdgeNode->GraphEdge;

			if (StartNode == nullptr || EndNode == nullptr || Edge == nullptr)
			{
				UE_LOG(LogToroGraphEd, Error, TEXT("UEdToroGraph::RebuildDataGraph add edge failed."));
				continue;
			}

			EdgeMap.Add(Edge, EdgeNode);

			Edge->OwningGraph = Graph;
			Edge->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
			Edge->StartNode = StartNode->GraphNode;
			Edge->EndNode = EndNode->GraphNode;
			Edge->StartNode->Edges.Add(Edge->EndNode, Edge);
		}
	}

	for (int i = 0; i < Graph->AllNodes.Num(); ++i)
	{
		UToroDataNode* Node = Graph->AllNodes[i];
		if (Node->ParentNodes.IsEmpty())
		{
			Graph->RootNodes.Add(Node);
			SortNodes(Node);
		}

		Node->OwningGraph = Graph;
		Node->Rename(nullptr, Graph, REN_DontCreateRedirectors | REN_DoNotDirty);
	}

	Graph->RootNodes.Sort([&](const UToroDataNode& L, const UToroDataNode& R)
	{
		const UEdToroGraphNode* EdNode_LNode = NodeMap[&L];
		const UEdToroGraphNode* EdNode_RNode = NodeMap[&R];
		return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
	});
}

UToroDataGraph* UEdToroGraph::GetGraph() const
{
	return CastChecked<UToroDataGraph>(GetOuter());
}

void UEdToroGraph::PostEditUndo()
{
	Super::PostEditUndo();
	NotifyGraphChanged();
}

bool UEdToroGraph::Modify(bool bAlwaysMarkDirty)
{
	const bool bSuper = Super::Modify(bAlwaysMarkDirty);
	GetGraph()->Modify();
	for (int32 i = 0; i < Nodes.Num(); ++i)
	{
		Nodes[i]->Modify();
	}
	return bSuper;
}

void UEdToroGraph::Clear()
{
	GetGraph()->ClearGraph();
	NodeMap.Reset();
	EdgeMap.Reset();

	/* Should be handled by GetGraph()->ClearGraph()
	for (int i = 0; i < Nodes.Num(); ++i)
	{
		const UEdToroGraphNode* EdNode = Cast<UEdToroGraphNode>(Nodes[i]);
		if (UToroDataNode* DataNode = EdNode ? EdNode->GraphNode : nullptr)
		{
			DataNode->ParentNodes.Reset();
			DataNode->ChildNodes.Reset();
			DataNode->Edges.Reset();
		}
	}*/
}

void UEdToroGraph::SortNodes(UToroDataNode* RootNode)
{
	int Level = 0;
	TArray<UToroDataNode*> CurrLevelNodes = { RootNode };
	TArray<UToroDataNode*> NextLevelNodes;
	TSet<UToroDataNode*> Visited;

	while (!CurrLevelNodes.IsEmpty())
	{
		for (int i = 0; i < CurrLevelNodes.Num(); ++i)
		{
			UToroDataNode* Node = CurrLevelNodes[i];
			Visited.Add(Node);

			auto PosSort = [&](const UToroDataNode& L, const UToroDataNode& R)
			{
				const UEdToroGraphNode* EdNode_LNode = NodeMap[&L];
				const UEdToroGraphNode* EdNode_RNode = NodeMap[&R];
				return EdNode_LNode->NodePosX < EdNode_RNode->NodePosX;
			};

			Node->ChildNodes.Sort(PosSort);
			Node->ParentNodes.Sort(PosSort);
			for (int j = 0; j < Node->ChildNodes.Num(); ++j)
			{
				if(!Visited.Contains(Node->ChildNodes[j]))
				{
					NextLevelNodes.Add(Node->ChildNodes[j]);
				}
			}
		}

		CurrLevelNodes = NextLevelNodes;
		NextLevelNodes.Reset();
		Level++;
	}
}
