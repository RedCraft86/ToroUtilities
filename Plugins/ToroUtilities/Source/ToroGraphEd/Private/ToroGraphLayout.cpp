// Copyright (C) RedCraft86. All Rights Reserved.

#include "ToroGraphLayout.h"
#include "Nodes/EdToroGraphNode.h"

void UToroGraphLayout::Layout(UEdGraph* InGraph)
{
	EdGraph = Cast<UEdToroGraph>(InGraph);
	check(EdGraph);

	EdGraph->RebuildDataGraph();
	Graph = EdGraph->GetGraph();
	check(Graph);

	const FVector2D Anchor(0.0f, 0.0f);
	for (int32 i = 0; i < Graph->RootNodes.Num(); ++i)
	{
		UToroDataNode* RootNode = Graph->RootNodes[i];
		InitPass(RootNode, Anchor);
		if (!bFirstPassOnly)
		{
			for (int32 j = 0; j < MaxIteration; ++j)
			{
				if (!ResolveConflictPass(RootNode)) break;
			}
		}
	}

	for (int32 i = 0; i < Graph->RootNodes.Num(); ++i)
	{
		for (int32 j = 0; j < i; ++j)
		{
			ResolveConflict(Graph->RootNodes[j], Graph->RootNodes[i]);
		}
	}

	Graph = nullptr;
	EdGraph = nullptr;
}

void UToroGraphLayout::InitPass(UToroDataNode* RootNode, const FVector2D& Anchor)
{
	UEdToroGraphNode* EdRootNode = EdGraph->NodeMap[RootNode];
	FVector2D ChildAnchor(FVector2D(0.0f, EdRootNode->GetNodeSize().Y + OptimalDistance + Anchor.Y));
	for (int32 i = 0; i < RootNode->ChildNodes.Num(); ++i)
	{
		UToroDataNode* Child = RootNode->ChildNodes[i];
		const UEdToroGraphNode* EdChildNode = EdGraph->NodeMap[Child];
		if (i > 0)
		{
			const UToroDataNode* PreChild = RootNode->ChildNodes[i - 1];
			const UEdToroGraphNode* EdPreChildNode = EdGraph->NodeMap[PreChild];
			ChildAnchor.X += OptimalDistance + EdPreChildNode->GetNodeSize().X / 2;
		}
		ChildAnchor.X += EdChildNode->GetNodeSize().X / 2;
		InitPass(Child, ChildAnchor);
	}

	EdRootNode->NodePosY = Anchor.Y;
	if (RootNode->ChildNodes.IsEmpty())
	{
		EdRootNode->NodePosX = Anchor.X - EdRootNode->GetNodeSize().X / 2;
	}
	else
	{
		UpdateParentNodePosition(RootNode);
	}
}

bool UToroGraphLayout::ResolveConflictPass(UToroDataNode* Node)
{
	bool HasConflict = false;
	for (int32 i = 0; i < Node->ChildNodes.Num(); ++i)
	{
		if (ResolveConflictPass(Node->ChildNodes[i]))
		{
			HasConflict = true;
		}
	}

	for (int32 i = 0; i < Node->ParentNodes.Num(); ++i)
	{
		UToroDataNode* ParentNode = Node->ParentNodes[i];
		for (int32 j = 0; j < ParentNode->ChildNodes.Num(); ++j)
		{
			UToroDataNode* LeftSibling = ParentNode->ChildNodes[j];
			if (LeftSibling == Node) break;
			if (ResolveConflict(LeftSibling, Node))
			{
				HasConflict = true;
			}
		}
	}

	return HasConflict;
}

bool UToroGraphLayout::ResolveConflict(UToroDataNode* LRoot, UToroDataNode* RRoot)
{
	TArray<UEdToroGraphNode*> RightContour, LeftContour;

	GetRightContour(LRoot, 0, RightContour);
	GetLeftContour(RRoot, 0, LeftContour);

	int32 MaxOverlapDistance = 0;
	const int32 Num = FMath::Min(LeftContour.Num(), RightContour.Num());
	for (int32 i = 0; i < Num; ++i)
	{
		if (RightContour.Contains(LeftContour[i]) || LeftContour.Contains(RightContour[i]))
			break;

		const int32 RightBound = RightContour[i]->NodePosX + RightContour[i]->GetNodeSize().X;
		const int32 Distance = RightBound + OptimalDistance - LeftContour[i]->NodePosX;
		if (Distance > MaxOverlapDistance)
		{
			MaxOverlapDistance = Distance;
		}
	}

	if (MaxOverlapDistance > 0)
	{
		ShiftSubTree(RRoot, FVector2D(MaxOverlapDistance, 0.0f));
		TArray<UToroDataNode*> ParentNodes = RRoot->ParentNodes;
		TArray<UToroDataNode*> NextParentNodes;
		while (!ParentNodes.IsEmpty())
		{
			for (int32 i = 0; i < ParentNodes.Num(); ++i)
			{
				UpdateParentNodePosition(ParentNodes[i]);
				NextParentNodes.Append(ParentNodes[i]->ParentNodes);
			}
			ParentNodes = NextParentNodes;
			NextParentNodes.Reset();
		}
		return true;
	}
	return false;
}

void UToroGraphLayout::GetLeftContour(UToroDataNode* RootNode, int32 Level, TArray<UEdToroGraphNode*>& Contour)
{
	UEdToroGraphNode* EdNode = EdGraph->NodeMap[RootNode];
	if (Level >= Contour.Num())
	{
		Contour.Add(EdNode);
	}
	else if (EdNode->NodePosX < Contour[Level]->NodePosX)
	{
		Contour[Level] = EdNode;
	}

	for (int32 i = 0; i < RootNode->ChildNodes.Num(); ++i)
	{
		GetLeftContour(RootNode->ChildNodes[i], Level + 1, Contour);
	}
}

void UToroGraphLayout::GetRightContour(UToroDataNode* RootNode, int32 Level, TArray<UEdToroGraphNode*>& Contour)
{
	UEdToroGraphNode* EdNode = EdGraph->NodeMap[RootNode];
	if (Level >= Contour.Num())
	{
		Contour.Add(EdNode);
	}
	else if (EdNode->NodePosX + EdNode->GetNodeSize().X > Contour[Level]->NodePosX + Contour[Level]->GetNodeSize().X)
	{
		Contour[Level] = EdNode;
	}

	for (int32 i = 0; i < RootNode->ChildNodes.Num(); ++i)
	{
		GetRightContour(RootNode->ChildNodes[i], Level + 1, Contour);
	}
}

void UToroGraphLayout::ShiftSubTree(UToroDataNode* RootNode, const FVector2D& Offset)
{
	UEdToroGraphNode* EdNode = EdGraph->NodeMap[RootNode];
	EdNode->NodePosX += Offset.X;
	EdNode->NodePosY += Offset.Y;

	for (int32 i = 0; i < RootNode->ChildNodes.Num(); ++i)
	{
		if (RootNode->ChildNodes[i]->ParentNodes[0] == RootNode)
		{
			ShiftSubTree(RootNode->ChildNodes[i], Offset);
		}
	}
}

void UToroGraphLayout::UpdateParentNodePosition(UToroDataNode* RootNode) const
{
	UEdToroGraphNode* EdParentNode = EdGraph->NodeMap[RootNode];
	if (RootNode->ChildNodes.Num() % 2 == 0)
	{
		const UEdToroGraphNode* FirstChild = EdGraph->NodeMap[RootNode->ChildNodes[0]];
		const UEdToroGraphNode* LastChild = EdGraph->NodeMap[RootNode->ChildNodes.Last()];
		const float RightBound = LastChild->NodePosX + LastChild->GetNodeSize().X;
		EdParentNode->NodePosX = (FirstChild->NodePosX + RightBound) / 2 - EdParentNode->GetNodeSize().X / 2;
	}
	else
	{
		const UEdToroGraphNode* MidChild = EdGraph->NodeMap[RootNode->ChildNodes[RootNode->ChildNodes.Num() / 2]];
		EdParentNode->NodePosX = MidChild->NodePosX + MidChild->GetNodeSize().X / 2 - EdParentNode->GetNodeSize().X / 2;
	}
}
