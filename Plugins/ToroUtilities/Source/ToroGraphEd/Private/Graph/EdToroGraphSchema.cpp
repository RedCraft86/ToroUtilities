// Copyright (C) RedCraft86. All Rights Reserved.

#include "Graph/EdToroGraphSchema.h"
#include "Framework/Commands/GenericCommands.h"
#include "Graph/ToroGraphConnectionPolicy.h"
#include "Nodes/EdToroGraphEdge.h"
#include "Nodes/EdToroGraphNode.h"
#include "Graph/EdToroGraph.h"
#include "GraphEditorActions.h"
#include "ToroDataGraph.h"

#define LOCTEXT_NAMESPACE "ToroGraphEd"

class FNodeVisitorCycleChecker
{
public:

	bool CheckForLoop(UEdGraphNode* StartNode, UEdGraphNode* EndNode)
	{
		VisitedNodes.Add(StartNode);
		return TraverseNodes(EndNode);
	}

private:
	
	bool TraverseNodes(UEdGraphNode* Node)
	{
		VisitedNodes.Add(Node);
		for (const UEdGraphPin* MyPin : Node->Pins)
		{
			if (MyPin->Direction == EGPD_Output)
			{
				for (const UEdGraphPin* OtherPin : MyPin->LinkedTo)
				{
					UEdGraphNode* OtherNode = OtherPin->GetOwningNode();
					if (VisitedNodes.Contains(OtherNode)) return false;
					if (!FinishedNodes.Contains(OtherNode))
					{
						if (!TraverseNodes(OtherNode)) return false;
					}
				}
			}
		}

		VisitedNodes.Remove(Node);
		FinishedNodes.Add(Node);
		return true;
	}
	
	TSet<UEdGraphNode*> VisitedNodes;
	TSet<UEdGraphNode*> FinishedNodes;
};

void FToroGraphAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

UEdGraphNode* FToroGraphAction_NewNode::PerformAction(UEdGraph* ParentGraph,
	UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;
	if (NodeTemplate)
	{
		const FScopedTransaction Transaction(LOCTEXT("NewNode", "ToroGraph Editor: New Node"));
		if (FromPin) FromPin->Modify();
		ParentGraph->Modify();

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);
		NodeTemplate->SetFlags(RF_Transactional);
		NodeTemplate->GraphNode->SetFlags(RF_Transactional);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void FToroGraphAction_NewEdge::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(NodeTemplate);
}

UEdGraphNode* FToroGraphAction_NewEdge::PerformAction(UEdGraph* ParentGraph,
	UEdGraphPin* FromPin, const FVector2f& Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;
	if (NodeTemplate)
	{
		const FScopedTransaction Transaction(LOCTEXT("NewEdge", "ToroGraph Editor: New Edge"));
		if (FromPin) FromPin->Modify();
		ParentGraph->Modify();

		NodeTemplate->Rename(nullptr, ParentGraph);
		ParentGraph->AddNode(NodeTemplate, true, bSelectNewNode);

		NodeTemplate->CreateNewGuid();
		NodeTemplate->PostPlacedNewNode();
		NodeTemplate->AllocateDefaultPins();
		NodeTemplate->AutowireNewNode(FromPin);
		NodeTemplate->SetFlags(RF_Transactional);
		NodeTemplate->GraphEdge->SetFlags(RF_Transactional);

		NodeTemplate->NodePosX = Location.X;
		NodeTemplate->NodePosY = Location.Y;

		ResultNode = NodeTemplate;
	}

	return ResultNode;
}

void UEdToroGraphSchema::GetBreakLinkToSubMenuActions(UToolMenu* Menu, UEdGraphPin* InGraphPin)
{
	TMap<FString, uint32> LinkTitleCount;
	FToolMenuSection& Section = Menu->FindOrAddSection("ToroGraphSchemaPinActions");
	for (TArray<UEdGraphPin*>::TConstIterator Links(InGraphPin->LinkedTo); Links; ++Links)
	{
		const UEdGraphPin* Pin = *Links;
		FString TitleString = Pin->GetOwningNode()->GetNodeTitle(ENodeTitleType::ListView).ToString();
		FText Title = FText::FromString(TitleString);
		if (Pin->PinName != TEXT(""))
		{
			TitleString = FString::Printf(TEXT("%s (%s)"), *TitleString, *Pin->PinName.ToString());

			FFormatNamedArguments Args;
			Args.Add(TEXT("NodeTitle"), Title);
			Args.Add(TEXT("PinName"), Pin->GetDisplayName());
			Title = FText::Format(LOCTEXT("BreakDescPin", "{NodeTitle} ({PinName})"), Args);
		}

		uint32& Count = LinkTitleCount.FindOrAdd(TitleString);

		FText Description;
		FFormatNamedArguments Args;
		Args.Add(TEXT("NodeTitle"), Title);
		Args.Add(TEXT("NumberOfNodes"), Count);

		if (Count == 0)
		{
			Description = FText::Format(LOCTEXT("BreakDesc", "Break link to {NodeTitle}"), Args);
		}
		else
		{
			Description = FText::Format(LOCTEXT("BreakDescMulti", "Break link to {NodeTitle} ({NumberOfNodes})"), Args);
		}
		Count++;

		Section.AddMenuEntry(NAME_None, Description, Description, FSlateIcon(), FUIAction(
			FExecuteAction::CreateUObject(this, &UEdToroGraphSchema::BreakSinglePinLink, InGraphPin, *Links)));
	}
}

EGraphType UEdToroGraphSchema::GetGraphType(const UEdGraph* TestEdGraph) const
{
	return GT_StateMachine;
}

void UEdToroGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	UToroDataGraph* Graph = CastChecked<UToroDataGraph>(ContextMenuBuilder.CurrentGraph->GetOuter());
	if (!Graph->CompatibleNode) return;

	TSet<TSubclassOf<UToroDataNode>> Visited;
	FText Desc = Graph->CompatibleNode.GetDefaultObject()->ContextMenuName;
	const FText AddToolTip = LOCTEXT("NewNodeTooltip", "Add node here");
	
	if (Desc.IsEmpty())
	{
		FString Title = Graph->CompatibleNode->GetName();
		Title.RemoveFromEnd("_C");
		Desc = FText::FromString(Title);
	}

	if (!Graph->CompatibleNode->HasAnyClassFlags(CLASS_Abstract))
	{
		const TSharedPtr<FToroGraphAction_NewNode> Action(new FToroGraphAction_NewNode(LOCTEXT("NodeAction", "Data Graph Node"), Desc, AddToolTip, 0));
		Action->NodeTemplate = NewObject<UEdToroGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
		Action->NodeTemplate->GraphNode = NewObject<UToroDataNode>(Action->NodeTemplate, Graph->CompatibleNode);
		Action->NodeTemplate->GraphNode->OwningGraph = Graph;
		ContextMenuBuilder.AddAction(Action);

		Visited.Add(Graph->CompatibleNode);
	}

	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(Graph->CompatibleNode) && !It->HasAnyClassFlags(CLASS_Abstract) && !Visited.Contains(*It))
		{
			TSubclassOf<UToroDataNode> NodeType = *It;
			if (It->GetName().StartsWith("REINST") || It->GetName().StartsWith("SKEL")) continue;
			if (!Graph->GetClass()->IsChildOf(NodeType.GetDefaultObject()->CompatibleGraph)) continue;

			Desc = NodeType.GetDefaultObject()->ContextMenuName;

			if (Desc.IsEmpty())
			{
				FString Title = NodeType->GetName();
				Title.RemoveFromEnd("_C");
				Desc = FText::FromString(Title);
			}

			TSharedPtr<FToroGraphAction_NewNode> Action(new FToroGraphAction_NewNode(LOCTEXT("NodeAction", "Data Graph Node"), Desc, AddToolTip, 0));
			Action->NodeTemplate = NewObject<UEdToroGraphNode>(ContextMenuBuilder.OwnerOfTemporaries);
			Action->NodeTemplate->GraphNode = NewObject<UToroDataNode>(Action->NodeTemplate, NodeType);
			Action->NodeTemplate->GraphNode->OwningGraph = Graph;
			ContextMenuBuilder.AddAction(Action);

			Visited.Add(NodeType);
		}
	}
}

void UEdToroGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	if (Context->Pin)
	{
		FToolMenuSection& Section = Menu->AddSection("ToroGraphSchemaActions", LOCTEXT("PinActionsMenuHeader", "Pin Actions"));
		if (!Context->Pin->LinkedTo.IsEmpty())
		{
			Section.AddMenuEntry(FGraphEditorCommands::Get().BreakPinLinks);
			if (Context->Pin->LinkedTo.Num() > 1)
			{
				Section.AddSubMenu("BreakLinkTo", LOCTEXT("BreakLinkTo", "Break Link To..."), LOCTEXT("BreakSpecificLinks", "Break a specific link..."),
					FNewToolMenuDelegate::CreateUObject((UEdToroGraphSchema* const)this, &UEdToroGraphSchema::GetBreakLinkToSubMenuActions, const_cast<UEdGraphPin*>(Context->Pin)));
			}
			else
			{
				((UEdToroGraphSchema* const)this)->GetBreakLinkToSubMenuActions(Menu, const_cast<UEdGraphPin*>(Context->Pin));
			}
		}
	}
	else if (Context->Node)
	{
		FToolMenuSection& Section = Menu->AddSection("ToroGraphSchemaActions", LOCTEXT("ClassActionsMenuHeader", "Node Actions"));
		Section.AddMenuEntry(FGenericCommands::Get().Delete);
		Section.AddMenuEntry(FGenericCommands::Get().Cut);
		Section.AddMenuEntry(FGenericCommands::Get().Copy);
		Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
		Section.AddMenuEntry(FGraphEditorCommands::Get().BreakNodeLinks);
	}

	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UEdToroGraphSchema::CanCreateConnection(const UEdGraphPin* A, const UEdGraphPin* B) const
{
	if (A->GetOwningNode() == B->GetOwningNode())
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorSameNode", "Can't connect node to itself"));
	}

	const UEdGraphPin* Out = A;
	const UEdGraphPin* In = B;

	const UEdToroGraphNode* EdNode_Out = Cast<UEdToroGraphNode>(Out->GetOwningNode());
	const UEdToroGraphNode* EdNode_In = Cast<UEdToroGraphNode>(In->GetOwningNode());
	if (EdNode_Out == nullptr || EdNode_In == nullptr)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinError", "Not a valid UEdToroGraphNode"));
	}

	bool bAllowCycles = false;
	if (const UEdToroGraph* EdGraph = Cast<UEdToroGraph>(Out->GetOwningNode()->GetGraph()))
	{
		bAllowCycles = EdGraph->GetGraph()->bCyclical;
	}
	
	FNodeVisitorCycleChecker CycleChecker;
	if (!bAllowCycles && !CycleChecker.CheckForLoop(Out->GetOwningNode(), In->GetOwningNode()))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, LOCTEXT("PinErrorCycle", "Can't create a graph cycle"));
	}

	FString ErrorMessage;
	if (!EdNode_Out->GraphNode->CanConnectTo(EdNode_In->GraphNode, EdNode_Out->GetOutputPin()->LinkedTo.Num(), &ErrorMessage))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
	}
	if (!EdNode_In->GraphNode->CanConnectFrom(EdNode_Out->GraphNode, EdNode_In->GetInputPin()->LinkedTo.Num(), &ErrorMessage))
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, ErrorMessage);
	}
	
	if (EdNode_Out->GraphNode->OwningGraph->CompatibleEdge)
	{
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, LOCTEXT("PinConnect", "Connect nodes with edge"));
	}
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, LOCTEXT("PinConnect", "Connect nodes"));
}

bool UEdToroGraphSchema::TryCreateConnection(UEdGraphPin* A, UEdGraphPin* B) const
{
	const UEdToroGraphNode* NodeA = Cast<UEdToroGraphNode>(A->GetOwningNode());
	const UEdToroGraphNode* NodeB = Cast<UEdToroGraphNode>(B->GetOwningNode());
	for (const UEdGraphPin* TestPin : NodeA->GetOutputPin()->LinkedTo)
	{
		UEdGraphNode* ChildNode = TestPin->GetOwningNode();
		if (UEdToroGraphEdge* EdNode_Edge = Cast<UEdToroGraphEdge>(ChildNode))
		{
			ChildNode = EdNode_Edge->GetEndNode();
		}

		if (ChildNode == NodeB) return false;
	}

	if (NodeA && NodeB)
	{
		Super::TryCreateConnection(NodeA->GetOutputPin(), NodeB->GetInputPin());
		return true;
	}
	
	return false;
}

bool UEdToroGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	UEdToroGraphNode* NodeA = Cast<UEdToroGraphNode>(A->GetOwningNode());
	UEdToroGraphNode* NodeB = Cast<UEdToroGraphNode>(B->GetOwningNode());
	if (!NodeA || !NodeA->GetOutputPin() || !NodeB || !NodeB->GetInputPin()) return false;

	const UToroDataGraph* Graph = NodeA->GraphNode->OwningGraph;
	const FVector2f InitPos((NodeA->NodePosX + NodeB->NodePosX) / 2, (NodeA->NodePosY + NodeB->NodePosY) / 2);

	FToroGraphAction_NewEdge Action;
	Action.NodeTemplate = NewObject<UEdToroGraphEdge>(NodeA->GetGraph());
	Action.NodeTemplate->SetEdge(NewObject<UToroDataEdge>(Action.NodeTemplate, Graph->CompatibleEdge));
	UEdToroGraphEdge* EdgeNode = Cast<UEdToroGraphEdge>(Action.PerformAction(
		NodeA->GetGraph(), nullptr, InitPos, false));
	EdgeNode->CreateConnections(NodeA, NodeB);

	return true;
}

FConnectionDrawingPolicy* UEdToroGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
	float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements, UEdGraph* InGraphObj) const
{
	return new FToroGraphConnectionPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect, InDrawElements, InGraphObj);
}

FLinearColor UEdToroGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

void UEdToroGraphSchema::BreakNodeLinks(UEdGraphNode& TargetNode) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakNodeLinks", "Break Node Links"));
	Super::BreakNodeLinks(TargetNode);
}

void UEdToroGraphSchema::BreakPinLinks(UEdGraphPin& TargetPin, bool bSendsNodeNotification) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakPinLinks", "Break Pin Links"));
	Super::BreakPinLinks(TargetPin, bSendsNodeNotification);
}

void UEdToroGraphSchema::BreakSinglePinLink(UEdGraphPin* SourcePin, UEdGraphPin* TargetPin) const
{
	const FScopedTransaction Transaction(NSLOCTEXT("UnrealEd", "GraphEd_BreakSinglePinLink", "Break Pin Link"));
	Super::BreakSinglePinLink(SourcePin, TargetPin);
}

UEdGraphPin* UEdToroGraphSchema::DropPinOnNode(UEdGraphNode* InTargetNode, const FName& InSourcePinName,
	const FEdGraphPinType& InSourcePinType, EEdGraphPinDirection InSourcePinDirection) const
{
	const UEdToroGraphNode* EdNode = Cast<UEdToroGraphNode>(InTargetNode);
	switch (InSourcePinDirection)
	{
	case EGPD_Input: return EdNode->GetOutputPin();
	case EGPD_Output: return EdNode->GetInputPin();
	default: return nullptr;
	}
}

bool UEdToroGraphSchema::SupportsDropPinOnNode(UEdGraphNode* InTargetNode, const FEdGraphPinType& InSourcePinType,
	EEdGraphPinDirection InSourcePinDirection, FText& OutErrorMessage) const
{
	return Cast<UEdToroGraphNode>(InTargetNode) != nullptr;
}

bool UEdToroGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return CurrentCacheRefreshID != InVisualizationCacheID;
}

int32 UEdToroGraphSchema::GetCurrentVisualizationCacheID() const
{
	return CurrentCacheRefreshID;
}

void UEdToroGraphSchema::ForceVisualizationCacheClear() const
{
	CurrentCacheRefreshID++;
}

#undef LOCTEXT_NAMESPACE